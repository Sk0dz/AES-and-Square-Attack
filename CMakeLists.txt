cmake_minimum_required(VERSION 2.6)
project (aes C)

include(CTest)
enable_testing()

add_library(aes STATIC aes.c gmath.c)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")
link_directories(${CMAKE_SOURCE_DIR})

add_executable(test_aes test_aes.c)
target_link_libraries(test_aes aes)

add_executable(test_gmath test_gmath.c)
target_link_libraries(test_gmath aes)

add_test(test_mix_columns_mult ./test_gmath mix_columns_mult)