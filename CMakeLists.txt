cmake_minimum_required(VERSION 3.18)
project(temp)
add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
set(CMAKE_CXX_STANDARD 14)

add_executable(app main.cpp dispatch.hpp groupCache.hpp cache.hpp lru.hpp cache.hpp consistent_hash.cpp consistent_hash.hpp)
#add_executable(app notes.cpp)

#add_executable(app main.cpp)

