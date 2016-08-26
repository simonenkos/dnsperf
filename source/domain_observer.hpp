#ifndef DNSPERF_DOMAIN_OBSERVER_HPP
#define DNSPERF_DOMAIN_OBSERVER_HPP

#include <condition_variable>
#include <atomic>
#include <future>
#include <thread>
#include <mutex>
#include <queue>
#include <map>

#include "query_result.hpp"
#include "domain_entry.hpp"
#include "storage_processor.hpp"

class domain_observer
{
   using qr_future  = std::future<query_result>;
   using domain_map = std::map<std::string, domain_entry>;
   
   std::queue<qr_future> results_;
   domain_map            domains_;
   
   mutable domain_map::const_iterator domain_iterator_;
   
   std::atomic_bool        is_stopped_ { false };
   std::condition_variable new_results_available_;
   
   std::thread observer_thread_;
   
   mutable std::mutex queries_mutex_;
   mutable std::mutex domains_mutex_;
   
   const storage_processor & processor_;

public:
   domain_observer(const storage_processor & processor);
   
   domain_observer(const domain_observer &  other) = delete;
   domain_observer(      domain_observer && other) = delete;
   
   domain_observer & operator=(const domain_observer &  other) = delete;
   domain_observer & operator=(      domain_observer && other) = delete;
   
   ~domain_observer() = default;
   
   void update(std::future<query_result> && result_future);
   void stop();
   
   // This method should be called before work to load data.
   bool load_domains();
   // The method allows to retrieve a observable domain in cyclic order.
   // When cycle is over function return false, othervise true;
   bool grab_domain (domain_entry & domain) const;
   
private:
   bool update_domain(const std::string & domain,
                      uint32_t latency);
   void run();
};

#endif // DNSPERF_DOMAIN_OBSERVER_HPP
