cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(stdral LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(stdral STATIC
  stdral.cpp
  stdral.h
)

target_compile_definitions(stdral PRIVATE LIBSTDRAL_LIBRARY)
