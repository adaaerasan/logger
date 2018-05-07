cmake_minimum_required(VERSION 3.10)
project(loglib)

set(CMAKE_CXX_STANDARD 11)

add_executable(loglib main.cpp logger.cpp logger.h logbuf.cpp logbuf.h )