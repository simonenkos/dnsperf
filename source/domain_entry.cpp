#include "domain_entry.hpp"

domain_entry::domain_entry(const std::string & url) : url_ { url }
{ }

domain_entry::domain_entry(domain_entry && other)
{
   url_ = std::move(other.url_);
   
   average_query_time_   = other.average_query_time_;
   query_time_deviation_ = other.query_time_deviation_;
   
   id_          = other.id_;
   query_count_ = other.query_count_;
   
   first_query_timestamp_ = other.final_query_timestamp_;
   final_query_timestamp_ = other.final_query_timestamp_;
}

domain_entry & domain_entry::operator=(domain_entry && other)
{
   url_ = std::move(other.url_);
   
   average_query_time_   = other.average_query_time_;
   query_time_deviation_ = other.query_time_deviation_;
   
   id_          = other.id_;
   query_count_ = other.query_count_;
   
   first_query_timestamp_ = other.final_query_timestamp_;
   final_query_timestamp_ = other.final_query_timestamp_;
   
   return * this;
}

inline const std::string & domain_entry::get_url() const
{
   return url_;
}

inline void domain_entry::set_average_query_time(float value)
{
   average_query_time_ = value;
}

inline float domain_entry::get_average_query_time() const
{
   return average_query_time_;
}

inline void domain_entry::set_query_time_deviation(float value)
{
   query_time_deviation_ =
}

inline float domain_entry::get_query_time_deviation() const
{
   return query_time_deviation_;
}

inline void domain_entry::set_query_count(uint64_t value)
{
   query_count_ = value;
}

inline uint64_t domain_entry::get_query_count() const
{
   return query_count_;
}

inline void domain_entry::set_first_query_timestamp(const date_time & value)
{
   first_query_timestamp_ = value;
}

inline const date_time & domain_entry::get_first_query_timestamp() const
{
   return first_query_timestamp_;
}

inline void domain_entry::set_final_query_timestamp(const date_time & value)
{
   final_query_timestamp_ = value;
}

inline const date_time & domain_entry::get_final_query_timestamp() const
{
   return final_query_timestamp_;
}