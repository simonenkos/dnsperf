#include "query_generator.hpp"

#include <algorithm>

namespace
{

const static std::string CHARSET                = "1234567890ABCDEF";
const static unsigned    MIN_RANDOM_DOMAIN_SIZE = 3;
const static unsigned    MAX_RANDOM_DOMAIN_SIZE = 9;
   
}

query_generator::query_generator() : random_engine_     { std::random_device {} },
                                     char_distribution_ { 0, CHARSET.size() - 1 },
                                     size_distribution_ { MIN_RANDOM_DOMAIN_SIZE, MAX_RANDOM_DOMAIN_SIZE }
{ }

std::future<query_result>
query_generator::generate(const std::string & url,
                          std::unique_ptr<query_processor> && query_processor_ptr,
                          std::unique_ptr<query_storage_processor> && query_storage_processor_ptr)
{
   auto random_domain_size = size_distribution_(random_engine_);
   
   std::string random_domain (random_domain_size + sizeof(char) + url.size(), 0);
   {
      std::generate_n(random_domain.begin(),
                      random_domain_size,
                      [this]
                      { return CHARSET[char_distribution_(random_engine_)]; });
      
      random_domain += '.';
      random_domain += url;
   }
   
   auto query = [random_domain,  qp_ptr = std::move(query_processor_ptr),
                                qsp_ptr = std::move(query_storage_processor_ptr)]
   {
      auto result = qp_ptr->process(random_domain);
      {
         qsp_ptr->process(result);
      }
      return result;
   };

   return std::async(std::move(query));
}