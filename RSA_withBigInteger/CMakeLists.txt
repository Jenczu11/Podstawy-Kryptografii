cmake_minimum_required(VERSION 3.12)
project(RSA)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS "-lgmp -lgmpxx")
set(CMAKE_CXX_FLAGS "-lgmp -lgmpxx")
add_executable(RSA main.cpp function.cpp function.h BigInteger.cpp BigInteger.h)
