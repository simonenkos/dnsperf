#include "domain_observer.hpp"

domain_observer::domain_observer()
{ }

void domain_observer::update(std::future <query_result> && result_future)
{
   std::lock_guard<std::mutex> lock { mutex_ };
   {
      results_.emplace(std::move(result_future));
   }
}

void domain_observer::run()
{
   std::unique_lock<std::mutex> lock { mutex_ };
   
   while (!results_.empty())
   {
      auto & result_future = results_.front();
      {
         lock.unlock();
      
         try
         {
            auto res = result_future.get();
            
            // ToDo ... add this to ...
         }
         catch (const std::exception & error)
         {
            // ToDo ...
         }
      
         lock.lock();
      }
      results_.pop();
   }
}