#ifndef DNSPERF_DOMAIN_ENTRY_HPP
#define DNSPERF_DOMAIN_ENTRY_HPP

#include <string>

#include <mysql++/datetime.h>

using date_time = mysqlpp::DateTime;

/**
 * POD type describing domain statistics data.
 */
struct domain_entry
{
   int32_t id { 0 };
   
   std::string url;
   
   double average_query_time   { 0 };
   double query_time_deviation { 0 };
    
   uint32_t query_count { 0UL };
   
   date_time first_query_timestamp;
   date_time final_query_timestamp;
};

#endif //DNSPERF_DOMAIN_ENTRY_HPP
