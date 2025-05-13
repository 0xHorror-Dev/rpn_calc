cmake_minimum_required(VERSION 3.16)

add_library(calc STATIC "src/calc.c" "test/c1.c")

target_include_directories(calc PUBLIC "inc/")

include_directories(../Collections-C/src/include)

target_link_libraries(calc PUBLIC collectc parser)

add_executable(ct1 "test/c1.c")
add_test(
    NAME test-ct1
    COMMAND $<TARGET_FILE:ct1>
)

target_link_libraries(ct1 calc parser)
