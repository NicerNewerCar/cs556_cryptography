cmake_minimum_required(VERSION 3.8)
project(rsa2 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)

find_library(NTL_LIB ntl)
if (NOT NTL_LIB)
  message(FATAL_ERROR "NTL not found!")
endif()

find_library(GMP_LIB gmp REQUIRED)
if (NOT GMP_LIB)
  message(FATAL_ERROR "GMP not found!")
endif()

add_executable(egml main.cpp) 
target_link_libraries(egml ${NTL_LIB} ${GMP_LIB})
