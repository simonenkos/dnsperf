#ifndef DNSPERF_DOMAIN_OBSERVER_HPP
#define DNSPERF_DOMAIN_OBSERVER_HPP

#include <future>
#include <thread>
#include <mutex>
#include <queue>

#include "query_result.hpp"
#include "domain_entry.hpp"

class domain_observer
{
   using qr_future = std::future<query_result>;
   
   std::queue<qr_future>     results_;
   std::vector<domain_entry> domains_;
   
   std::thread observer_thread_;
   std::mutex  mutex_;

public:
   domain_observer();
   
   domain_observer(const domain_observer &  other) = delete;
   domain_observer(      domain_observer && other) = delete;
   
   domain_observer & operator=(const domain_observer &  other) = delete;
   domain_observer & operator=(      domain_observer && other) = delete;
   
   virtual ~domain_observer() = default;
   
   void update(std::future<query_result> && result_future);
   
private:
   void run();
};

#endif // DNSPERF_DOMAIN_OBSERVER_HPP
