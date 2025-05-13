cmake_minimum_required(VERSION 3.16)

add_library(calc STATIC "src/calc.c" "test/evaluation_test.c")

target_include_directories(calc PUBLIC "inc/")

include_directories(../Collections-C/src/include)

target_link_libraries(calc PUBLIC collectc parser)

add_executable(evaluation_test "test/evaluation_test.c")
add_test(
    NAME test-evaluation_test
    COMMAND $<TARGET_FILE:evaluation_test>
)

target_link_libraries(evaluation_test calc parser)
