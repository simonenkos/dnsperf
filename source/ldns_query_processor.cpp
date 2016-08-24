#include <exception>
#include <ldns/ldns.h>
#include <stdexcept>

#include "ldns_query_processor.hpp"

namespace media
{

ldns_query_processor::ldns_query_processor()
{}

query_result ldns_query_processor::process(const std::string & url)
{
   auto domain_ptr = ldns_dname_new_frm_str(url.c_str());
   
   if (!domain_ptr)
      throw std::runtime_error { "ldns_dname_new_frm_str failure" };
   
   ldns_resolver * resolver_ptr { nullptr };
   
   if (ldns_resolver_new_frm_file(&resolver_ptr, nullptr) != LDNS_STATUS_OK)
      throw std::runtime_error { "ldns_resolver_new_frm_file failure failure" };
   
   auto packet = ldns_resolver_query(resolver_ptr,
                                     domain_ptr,
                                     LDNS_RR_TYPE_A,
                                     LDNS_RR_CLASS_IN,
                                     LDNS_RD);
   ldns_rdf_deep_free(domain_ptr);
   
   if (!packet)
      throw std::runtime_error { "ldns_resolver_query failure" };
   
   query_result result;
   {
      result.url     = url;
      result.latency = ldns_pkt_querytime(packet);
   }
   return result;
}

} // namespace media