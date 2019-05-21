cmake_minimum_required(VERSION 3.12)
project(RSA)
set(CMAKE_CXX_STANDARD 14)

add_executable(RSA main.cpp function.cpp function.h)
target_link_libraries(RSA gmp libgmp gmpxx)