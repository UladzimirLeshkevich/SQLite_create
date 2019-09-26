cmake_minimum_required(VERSION 3.5)

project(sql_create LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(sqlite3 SHARED sqlite3.h sqlite3.c)

add_executable(sql_create main.cpp)

target_link_libraries(sql_create sqlite3)
