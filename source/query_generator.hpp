#ifndef DNSPERF_QUERY_GENERATOR_HPP
#define DNSPERF_QUERY_GENERATOR_HPP

#include <future>
#include <random>
#include <memory>

#include "query_processor.hpp"
#include "storage_processor.hpp"

class query_generator
{
   std::default_random_engine               random_engine_;
   std::uniform_int_distribution<unsigned > char_distribution_;
   std::uniform_int_distribution<unsigned > size_distribution_;
   
public:
   query_generator();
   
   query_generator(const query_generator &  other) = delete;
   query_generator(      query_generator && other) = delete;
   
   query_generator & operator=(const query_generator &  other) = delete;
   query_generator & operator=(      query_generator && other) = delete;
   
   ~query_generator() = default;
   
   std::future<query_result> generate(const domain_entry      & domain,
                                      const query_processor   & query_processor,
                                      const storage_processor & storage_processor);
};

#endif //DNSPERF_QUERY_GENERATOR_HPP
