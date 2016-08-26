#include "query_generator.hpp"

#include <algorithm>

namespace
{

const static std::string CHARSET                = "1234567890"
                                                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                  "abcdefghijklmnopqrstuvwxyz";
const static unsigned    MIN_RANDOM_DOMAIN_SIZE = 3;
const static unsigned    MAX_RANDOM_DOMAIN_SIZE = 9;
   
}

query_generator::query_generator() : char_distribution_ { 0, static_cast<unsigned>(CHARSET.size() - 1) },
                                     size_distribution_ { MIN_RANDOM_DOMAIN_SIZE, MAX_RANDOM_DOMAIN_SIZE }
{ }

std::future<query_result>
query_generator::generate(const domain_entry      & domain,
                          const query_processor   & query_processor,
                          const storage_processor & storage_processor)
{
   auto random_domain_url_size = size_distribution_(random_engine_);
   auto domain_url             = domain.url;
   
   std::string random_domain_url (random_domain_url_size + sizeof(char) + domain_url.size(), 0);
   {
      std::generate_n(random_domain_url.begin(),
                      random_domain_url_size,
                      [this]
                      { return CHARSET[char_distribution_(random_engine_)]; });
      
      random_domain_url += '.';
      random_domain_url += domain_url;
   }
   
   auto query = [random_domain_url, domain, & query_processor, & storage_processor]
   {
      auto result = query_processor.process(random_domain_url);
      {
         storage_processor.process_query_data(result, domain);
      }
      return result;
   };

   return std::async(std::move(query));
}