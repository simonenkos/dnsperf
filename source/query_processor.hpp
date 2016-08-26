#ifndef DNSPERF_QUERY_PROCESSOR_HPP
#define DNSPERF_QUERY_PROCESSOR_HPP

#include <string>
#include <cstdint>

#include "query_result.hpp"

/**
 * Interface of processor which is responsible for making a query.
 */
struct query_processor
{
   query_processor() = default;
   
   query_processor(const query_processor &  other) = default;
   query_processor(      query_processor && other) = default;
   
   query_processor & operator=(const query_processor &  other) = default;
   query_processor & operator=(      query_processor && other) = default;
   
   virtual ~query_processor() = default;
   
   virtual query_result process(const std::string & url) const = 0;
};

#endif //DNSPERF_QUERY_PROCESSOR_HPP
