#ifndef DNSPERF_MYSQL_STORAGE_PROCESSOR_HPP
#define DNSPERF_MYSQL_STORAGE_PROCESSOR_HPP

#include "query_storage_processor.hpp"

class mysql_storage_processor : public query_storage_processor
{
public:
   mysql_storage_processor();
   
   mysql_storage_processor(const mysql_storage_processor &  other) = default;
   mysql_storage_processor(      mysql_storage_processor && other) = default;
   
   mysql_storage_processor & operator=(const mysql_storage_processor &  other) = default;
   mysql_storage_processor & operator=(const mysql_storage_processor && other) = default;
   
   virtual ~mysql_storage_processor() = default;
   
   virtual void process_query(const query_result & qr) override;
};

#endif //DNSPERF_MYSQL_STORAGE_PROCESSOR_HPP
