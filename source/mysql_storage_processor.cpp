#include <iostream>

#include <mysql++/connection.h>
#include <mysql++/query.h>

#include "mysql_storage_processor.hpp"

mysql_storage_processor::mysql_storage_processor(const std::string & db_name,
                                                 const std::string & db_host,
                                                 const std::string & username,
                                                 const std::string & password) : db_name_  { db_name  },
                                                                                 db_host_  { db_host  },
                                                                                 username_ { username },
                                                                                 password_ { password }
{ }

bool mysql_storage_processor::process_query_data(const query_result & qr,
                                                 const domain_entry & domain) const
{
   mysqlpp::Connection connection { false };
   
   if (!connection.connect(db_name_.c_str(), db_host_.c_str(), username_.c_str(), password_.c_str()))
   {
      std::cerr << "mysql_storage_processor: cannot connect to database" << std::endl;
      return false;
   }
   
   mysqlpp::Query query { & connection };
   {
      query << "insert into queries (latency, domain_id) values ('" << qr.latency << "','" << domain.id << "')";
   }
   
   try
   {
      query.store();
      return true;
   }
   catch (std::exception & error)
   {
      std::cerr << "mysql_storage_processor: cannot process query: "
                << domain.url << ": " << error.what() << std::endl;
      return false;
   }
}

bool mysql_storage_processor::load_domains(std::map<std::string, domain_entry> & domains) const
{
   mysqlpp::Connection connection { false };
   
   if (!connection.connect(db_name_.c_str(), db_host_.c_str(), username_.c_str(), password_.c_str()))
   {
      std::cerr << "mysql_storage_processor: cannot connect to database" << std::endl;
      return false;
   }
   
   mysqlpp::Query query { & connection, false };
   {
      query << "select * from domains";
   }
   
   auto result = query.store();
   
   if (!result)
   {
      std::cerr << "mysql_storage_processor: cannot store query result!" << std::endl;
      return false;
   }
      
   for (auto index = 0U; index < result.num_rows(); index++)
   {
      domain_entry domain;
      
      try
      {
         domain.id                    = int32_t    (result[index]["id"]);
         domain.url                   = std::string(result[index]["url"]);
         domain.average_query_time    = double     (result[index]["average_query_time"]);
         domain.query_time_deviation  = double     (result[index]["query_time_deviation"]);
         domain.query_count           = uint32_t   (result[index]["query_count"]);
         domain.first_query_timestamp = date_time  (result[index]["first_query_timestamp"]);
         domain.final_query_timestamp = date_time  (result[index]["final_query_timestamp"]);
      }
      catch (mysqlpp::BadConversion & error)
      {
         std::cerr << "mysql_storage_processor: bad conversion: " << error.what() << std::endl;
         return false;
      }
      
      domains.emplace(domain.url, domain);
   }
   
   return true;
}

bool mysql_storage_processor::update_domain(const domain_entry & domain) const
{
   mysqlpp::Connection connection { false };
   
   if (!connection.connect(db_name_.c_str(), db_host_.c_str(), username_.c_str(), password_.c_str()))
   {
      std::cerr << "mysql_storage_processor: cannot connect to database" << std::endl;
      return false;
   }
   
   mysqlpp::Query query { & connection };
   {
      query << "update domains set "
            << "domains.url='"                   << domain.url                   << "',"
            << "domains.average_query_time='"    << domain.average_query_time    << "',"
            << "domains.query_time_deviation='"  << domain.query_time_deviation  << "',"
            << "domains.query_count='"           << domain.query_count           << "',"
            << "domains.first_query_timestamp='" << domain.first_query_timestamp << "',"
            << "domains.final_query_timestamp='" << domain.final_query_timestamp << "' where "
            << "domains.id='"                    << domain.id                    << "'";
   }
   
   try
   {
      query.store();
      return true;
   }
   catch (std::exception & error)
   {
      std::cerr << "mysql_storage_processor: cannot update domain: "
                << domain.url << ": " << error.what() << std::endl;
      return false;
   }
}