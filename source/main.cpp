#include <iostream>
#include <vector>
#include <queue>

#include <boost/program_options.hpp>

#include "domain_entry.hpp"
#include "query_generator.hpp"
#include "ldns_query_processor.hpp"
#include "domain_observer.hpp"

int main(int argc, char ** argv)
{
   namespace bpo = boost::program_options;
   
   bpo::option_description desc;
   bpo::variables_map vm;
   
   desc.add_options()
      ("help,h",      "print help")
      ("frequency,f", "frequency of dns query generation");
   
   try
   {
      bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
      
      if (vm.count("help"))
      {
         std::cout << desc << std::endl;
         return EXIT_SUCCESS;
      }
   }
   catch (const std::exception & error)
   {
      std::cerr << error.what() << std::endl;
      return EXIT_FAILURE;
   }
   
   // Load data from 'domains' table.
   std::vector<domain_entry> domains_statistics;
   
   // Setup timeout function...
   
   query_generator gen;
   
   while (true)
   {
      std::this_thread::sleep_for(1s);
      
      for (const auto & domain_info : domains_statistics)
      {
         auto result = gen.generate(domain_info.get_url(),
                                    std::make_unique<ldns_query_processor>(),
                                    std::make_unique<mysql_storage_processor>()); // ToDo
         domain_observer.update(result);
      }
   }
   
   return EXIT_SUCCESS;
}