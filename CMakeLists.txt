cmake_minimum_required(VERSION 3.5)

project(dnsperf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

find_package(Boost REQUIRED program_options)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "No Boost library was found!")
endif()

set(SOURCE_FILES source/main.cpp
                 source/domain_entry.hpp
                 source/domain_entry.cpp
                 source/query_generator.hpp
                 source/query_generator.cpp
                 source/query_processor.hpp
                 source/query_result.hpp
                 source/ldns_query_processor.hpp
                 source/ldns_query_processor.cpp)

add_executable(dnsperf ${SOURCE_FILES})