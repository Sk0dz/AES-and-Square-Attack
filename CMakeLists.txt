cmake_minimum_required(VERSION 2.6)
project (aes C)

include(CTest)
enable_testing()

add_library(aes STATIC aes.c gmath.c attack.c)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")
link_directories(${CMAKE_SOURCE_DIR})

add_executable(main main.c)
target_link_libraries(main aes)

add_executable(main_attack main_attack.c)
target_link_libraries(main_attack aes)

add_executable(test_aes test_aes.c)
target_link_libraries(test_aes aes)

add_executable(test_gmath test_gmath.c)
target_link_libraries(test_gmath aes)

add_test(test_rot_word ./test_aes rot_word)
add_test(test_sub_word ./test_aes sub_word)
add_test(test_key_expansion ./test_aes key_expansion)
add_test(test_sub_bytes ./test_aes sub_bytes)
add_test(test_inverse_sub_bytes ./test_aes inverse_sub_bytes)
add_test(test_shift_rows ./test_aes shift_rows)
add_test(test_inverse_shift_rows ./test_aes inverse_shift_rows)
add_test(test_mix_columns ./test_aes mix_columns)
add_test(test_inverse_mix_columns ./test_aes inverse_mix_columns)

add_test(test_mix_columns_mult ./test_gmath mix_columns_mult)
add_test(test_inverse_mix_columns_mult ./test_gmath inverse_mix_columns_mult)