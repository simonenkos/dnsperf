#ifndef DNSPERF_DOMAIN_STORAGE_PROCESSOR_HPP
#define DNSPERF_DOMAIN_STORAGE_PROCESSOR_HPP

struct domain_storage_processor // ToDo! not very good
{
   domain_storage_processor() = default;
   
   domain_storage_processor(const domain_storage_processor &  other) = default;
   domain_storage_processor(      domain_storage_processor && other) = default;
   
   domain_storage_processor & operator=(const domain_storage_processor &  other) = default;
   domain_storage_processor & operator=(      domain_storage_processor && other) = default;
   
   virtual ~domain_storage_processor() = default;
   
//   virtual void save_domain();
//   virtual void load_domain();
};

#endif // DNSPERF_DOMAIN_STORAGE_PROCESSOR_HPP
