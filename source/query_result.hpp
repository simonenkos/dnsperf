#ifndef DNSPERF_QUERY_RESULT_HPP
#define DNSPERF_QUERY_RESULT_HPP

struct query_result
{
   std::string url;
   uint32_t    latency { 0ULL };
};

#endif // DNSPERF_QUERY_RESULT_HPP
