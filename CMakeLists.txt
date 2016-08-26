cmake_minimum_required(VERSION 3.5)

project(dnsperf)

add_definitions(-DMYSQLPP_MYSQL_HEADERS_BURIED)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Boost REQUIRED program_options)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "No Boost library was found!")
endif()

find_package(MySQLPP REQUIRED)
if (NOT MYSQLPP_FOUND)
    message(FATAL_ERROR "No MySQL++ libraty was found!")
endif()

find_package(LDNS REQUIRED)
if (NOT LDNS_FOUND)
    message(FATAL_ERROR "No ldns library was found!")
endif()

find_package(Threads)

set(SOURCE_FILES source/main.cpp
                 source/domain_entry.hpp
                 source/query_result.hpp
                 source/query_processor.hpp
                 source/storage_processor.hpp
                 source/domain_observer.hpp
                 source/domain_observer.cpp
                 source/query_generator.hpp
                 source/query_generator.cpp
                 source/ldns_query_processor.hpp
                 source/ldns_query_processor.cpp
                 source/mysql_storage_processor.hpp
                 source/mysql_storage_processor.cpp)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${MYSQLPP_INCLUDE_DIRS})
include_directories(${LDNS_LIBRARIES})

add_executable(dnsperf ${SOURCE_FILES})

target_link_libraries(dnsperf ${Boost_LIBRARIES}
                              ${MYSQLPP_LIBRARIES}
                              ${LDNS_LIBRARIES}
                              ${CMAKE_THREAD_LIBS_INIT})