#ifndef DNSPERF_STORAGE_PROCESSOR_HPP
#define DNSPERF_STORAGE_PROCESSOR_HPP

#include <map>

#include "query_result.hpp"
#include "domain_entry.hpp"

struct storage_processor
{
   storage_processor() = default;
   
   storage_processor(const storage_processor &  other) = default;
   storage_processor(      storage_processor && other) = default;
   
   storage_processor & operator=(const storage_processor &  other) = default;
   storage_processor & operator=(      storage_processor && other) = default;
   
   virtual ~storage_processor() = default;
   
   virtual bool process_query_data(const query_result & qr,
                                   const domain_entry & domain) const = 0;
   virtual bool load_domains(std::map<std::string, domain_entry> & domains) const = 0;
   virtual bool update_domain(const domain_entry & domains) const = 0;
};

#endif // DNSPERF_QUERY_STORAGE_PROCESSOR_HPP
