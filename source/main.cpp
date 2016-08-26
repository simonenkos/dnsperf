#include <iostream>
#include <csignal>
#include <chrono>

#include <boost/program_options.hpp>

#include "query_generator.hpp"
#include "ldns_query_processor.hpp"
#include "mysql_storage_processor.hpp"
#include "domain_observer.hpp"

bool stop_flag = false;

void signal_handler(int signal)
{
   stop_flag = true;
}

int main(int argc, const char * const * argv)
{
   std::string db_name { "dnsperfdb" },
               db_host { "localhost" };
   std::string username, password;
   
   uint32_t frequency { 0 };
   
   namespace bpo = boost::program_options;
   
   bpo::options_description desc;
   bpo::variables_map vm;
   
   desc.add_options()
      ("help,h",      "print help")
      ("frequency,f", bpo::value<unsigned>(),    "frequency of dns query generation, seconds")
      ("database,d",  bpo::value<std::string>(), "name of the database to connect (optional)")
      ("host",        bpo::value<std::string>(), "address of a host where the database is located (optional)")
      ("username,u",  bpo::value<std::string>(), "username for database to connect")
      ("password,p",  bpo::value<std::string>(), "password for database to connect");
   
   try
   {
      bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
      
      if (vm.count("help"))
      {
         std::cout << desc << std::endl;
         return EXIT_SUCCESS;
      }
      
      if (vm.count("frequency"))
         frequency = vm["frequency"].as<unsigned>();
      else
      {
         std::cerr << "please, enter a frequency for DNS query generation..." << std::endl;
         return EXIT_FAILURE;
      }
      
      if (vm.count("database"))
         db_name = vm["database"].as<std::string>();
      
      if (vm.count("host"))
         db_host = vm["host"].as<std::string>();
      
      if (vm.count("username"))
         username = vm["username"].as<std::string>();
      else
      {
         std::cerr << "please, enter a username to connect to the database..." << std::endl;
         return EXIT_FAILURE;
      }
      
      if (vm.count("password"))
         password = vm["password"].as<std::string>();
      else
      {
         std::cerr << "please, enter a password to connect to the database..." << std::endl;
         return EXIT_FAILURE;
      }
   }
   catch (const std::exception & error)
   {
      std::cerr << error.what() << std::endl;
      return EXIT_FAILURE;
   }
   
   std::signal(SIGINT,  signal_handler);
   std::signal(SIGTERM, signal_handler);
   
   query_generator gen;
   ldns_query_processor query_proc;
   mysql_storage_processor storage_proc { db_name, db_host, username, password };
   domain_observer observer { storage_proc };
   
   if (!observer.load_domains())
   {
      std::cerr << "cannot load domains data from database...";
      return EXIT_FAILURE;
   }
   
   while (!stop_flag)
   {
      domain_entry domain;
      
      while (observer.grab_domain(domain))
      {
         auto result = gen.generate(domain,
                                    query_proc,
                                    storage_proc);
         observer.update(std::move(result));
      }
      
      std::this_thread::sleep_for(std::chrono::seconds(frequency));
   }
   
   observer.stop();
   std::cout << "stopped..." << std::endl;
   return EXIT_SUCCESS;
}