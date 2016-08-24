#ifndef DNSPERF_DOMAIN_ENTRY_HPP
#define DNSPERF_DOMAIN_ENTRY_HPP

#include <string>

#include <mysql++/datetime.h>

using date_time = mysqlpp::DateTime;

class domain_entry
{
   std::string url_;
   
   float average_query_time_   { 0F };
   float query_time_deviation_ { 0F };
   
   uint64_t id_          { 0ULL };
   uint64_t query_count_ { 0ULL };
   
   date_time first_query_timestamp_;
   date_time final_query_timestamp_;
   
public:
   domain_entry(const std::string & url);
   
   domain_entry(const domain_entry &  other) = default;
   domain_entry(      domain_entry && other);
   
   domain_entry & operator=(const domain_entry &  other) = default;
   domain_entry & operator=(      domain_entry && other);
   
   ~domain_entry() = default;
   
   const std::string & get_url() const noexcept;
   
   void  set_average_query_time(float value) noexcept;
   float get_average_query_time() const noexcept;
   
   void  set_query_time_deviation(float value) noexcept;
   float get_query_time_deviation() const noexcept;
   
   void     set_query_count(uint64_t value) noexcept;
   uint64_t get_query_count() const noexcept;
   
   void              set_first_query_timestamp(const date_time & value);
   const date_time & get_first_query_timestamp() const noexcept;
   
   void              set_final_query_timestamp(const date_time & value);
   const date_time & get_final_query_timestamp() const noexcept;
};

#endif //DNSPERF_DOMAIN_ENTRY_HPP
