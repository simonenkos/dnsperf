#ifndef DNSPERF_MYSQL_STORAGE_PROCESSOR_HPP
#define DNSPERF_MYSQL_STORAGE_PROCESSOR_HPP

#include "storage_processor.hpp"

class mysql_storage_processor : public storage_processor
{
   const std::string db_name_;
   const std::string db_host_;
   const std::string username_;
   const std::string password_;
   
public:
   mysql_storage_processor(const std::string & db_name,
                           const std::string & db_host,
                           const std::string & username,
                           const std::string & password);
   
   mysql_storage_processor(const mysql_storage_processor &  other) = default;
   mysql_storage_processor(      mysql_storage_processor && other) = default;
   
   mysql_storage_processor & operator=(const mysql_storage_processor &  other) = delete;
   mysql_storage_processor & operator=(const mysql_storage_processor && other) = delete;
   
   virtual ~mysql_storage_processor() = default;
   
   virtual bool process_query_data(const query_result & qr,
                                   const domain_entry & domain) const override;
   virtual bool load_domains(std::map<std::string, domain_entry> & domains) const override;
   virtual bool update_domain(const domain_entry & domain) const override;
};

#endif //DNSPERF_MYSQL_STORAGE_PROCESSOR_HPP
