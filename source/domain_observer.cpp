#include <mysql++/connection.h>
#include <mysql++/query.h>

#include <chrono>
#include <cmath>

#include "domain_observer.hpp"

domain_observer::domain_observer(const storage_processor & processor) : processor_ { processor }
{
   observer_thread_ = std::thread { & domain_observer::run, this };
}

void domain_observer::update(std::future <query_result> && result_future)
{
   std::lock_guard<std::mutex> lock { queries_mutex_ };
   {
      results_.emplace(std::move(result_future));
      new_results_available_.notify_one();
   }
}

void domain_observer::stop()
{
   is_stopped_ = true;
   // To prevent waiting forever.
   new_results_available_.notify_one();
   
   if (observer_thread_.joinable()) observer_thread_.join();
}

bool domain_observer::load_domains()
{
   if (!processor_.load_domains(domains_))
      return false;
   
   domain_iterator_ = domains_.begin();
   return true;
}

bool domain_observer::grab_domain(domain_entry & domain) const
{
   if (domains_.empty())
      throw std::runtime_error { "domain observer was not properly initialized" };
   
   if (domain_iterator_ == domains_.end())
   {
      domain_iterator_ = domains_.begin();
      return false;
   }
   
   std::lock_guard<std::mutex> lock { domains_mutex_ };
   {
      domain = domain_iterator_->second;
      domain_iterator_++;
   }
   return true;
}

bool domain_observer::update_domain(const std::string & domain, uint32_t latency)
{
   auto domain_it = domains_.find(domain);
   
   if (domain_it == domains_.end())
      return false;
   
   domain_entry tmp_domain = domain_it->second;
   
   if (tmp_domain.query_count == 0)
   {
      tmp_domain.average_query_time    = double(latency);
      tmp_domain.query_time_deviation  = double(latency);
      tmp_domain.first_query_timestamp = date_time { std::time(nullptr) };
   }
   else
   {
      auto old_average_query_time =  tmp_domain.average_query_time;
      auto new_average_query_time = (tmp_domain.average_query_time + double(latency)) / 2;
      auto query_count            =  tmp_domain.query_count + 1;
      
      if (tmp_domain.query_count == 1)
      {
         // Calculate deviation for two element.
         tmp_domain.query_time_deviation = sqrt(pow(tmp_domain.query_time_deviation - new_average_query_time, 2.0) +
                                                pow(latency - new_average_query_time, 2.0)) / query_count;
      }
      else
      {
         // Calculate deviation using previous value.
         // http://math.stackexchange.com/questions/775391/can-i-calculate-the-new-standard-deviation-when-adding-a-value-without-knowing-t
         tmp_domain.query_time_deviation = sqrt(((query_count - 2) * pow(tmp_domain.query_time_deviation, 2.0) +
                                            (latency - new_average_query_time) *
                                            (latency - old_average_query_time)) / (query_count - 1));
      }
      
      tmp_domain.average_query_time = new_average_query_time;
   }
   
   tmp_domain.final_query_timestamp = date_time { std::time(nullptr) };
   tmp_domain.query_count++;
   
   std::lock_guard<std::mutex> lock { domains_mutex_ };
   {
      domain_it->second = tmp_domain;
   }
   
   return processor_.update_domain(tmp_domain);
}

void domain_observer::run()
{
   while (!is_stopped_)
   {
      qr_future fut;
      {
         std::unique_lock<std::mutex> lock { queries_mutex_ };
   
         new_results_available_.wait(lock);
   
         if (results_.empty()) continue;
         
         fut = std::move(results_.front());
         {
            results_.pop();
         }
      }
   
      try
      {
         auto qr = fut.get();
         
         auto dot_position = qr.url.find('.');
         auto domain_name  = qr.url.substr(dot_position + 1);
         
         if (!update_domain(domain_name, qr.latency))
         {
            std::cerr << "cannot update domain: " << domain_name << std::endl;
            continue;
         }
      }
      catch (const std::exception & error)
      {
         // ToDo: Save exception to return for update method to inform main thread about errors.
         std::cerr << error.what() << std::endl;
      }
   }
   
   while (!results_.empty()) results_.pop();
}