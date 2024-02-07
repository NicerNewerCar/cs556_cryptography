cmake_minimum_required(VERSION 3.8)
project(rsa LANGUAGES CXX)

find_library(NTL_LIB ntl)
if (NOT NTL_LIB)
  message(FATAL_ERROR "NTL not found!")
endif()

add_executable(rsa main.cpp)
