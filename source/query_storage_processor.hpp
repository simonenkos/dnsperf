#ifndef DNSPERF_QUERY_STORAGE_PROCESSOR_HPP
#define DNSPERF_QUERY_STORAGE_PROCESSOR_HPP

#include "query_result.hpp"

struct query_storage_processor
{
   query_storage_processor() = default;
   
   query_storage_processor(const query_storage_process &  other) = default;
   query_storage_processpr(      query_storage_process && other) = default;
   
   query_storage_processor & operator=(const query_storage_process &  other) = default;
   query_storage_processor & operator=(      query_storage_process && other) = default;
   
   virtual ~query_storage_processor() = default;
   
   virtual void process_query(const query_result & qr) = 0;
};

#endif // DNSPERF_QUERY_STORAGE_PROCESSOR_HPP
