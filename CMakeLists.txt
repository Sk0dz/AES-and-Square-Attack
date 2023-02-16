cmake_minimum_required(VERSION 2.6)
project (aes C)

include(CTest)
enable_testing()

add_library(aes STATIC aes.c gmath.c)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")
link_directories(${CMAKE_SOURCE_DIR})

add_executable(aes_test aes_test.c)
target_link_libraries(aes_test aes)

add_executable(gmath_test gmath_test.c)
target_link_libraries(gmath_test aes)