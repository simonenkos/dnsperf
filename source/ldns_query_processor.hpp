#ifndef DNSPERF_LDNS_QUERY_PROCESSOR_HPP
#define DNSPERF_LDNS_QUERY_PROCESSOR_HPP

#include "query_processor.hpp"

class ldns_query_processor : public query_processor
{
public:
   ldns_query_processor();
   
   ldns_query_processor(const ldns_query_processor &  other) = default;
   ldns_query_processor(      ldns_query_processor && other) = default;
   
   ldns_query_processor & operator=(const ldns_query_processor &  other) = default;
   ldns_query_processor & operator=(      ldns_query_processor && other) = default;
   
   virtual ~ldns_query_processor() = default;
   
   virtual query_result process(const std::string & url) override;
};

#endif //DNSPERF_LDNS_QUERY_PROCESSOR_HPP
