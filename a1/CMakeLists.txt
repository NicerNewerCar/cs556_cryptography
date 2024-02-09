cmake_minimum_required(VERSION 3.8)
project(rsa LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)

find_library(NTL_LIB ntl)
if (NOT NTL_LIB)
  message(FATAL_ERROR "NTL not found!")
endif()

find_library(GMP_LIB gmp REQUIRED)
if (NOT GMP_LIB)
  message(FATAL_ERROR "GMP not found!")
endif()

add_executable(rsa main.cpp rsa.hpp rsa.cpp brute_force.hpp brute_force.cpp) 
target_link_libraries(rsa ${NTL_LIB} ${GMP_LIB})
