

include_directories(../Collections-C/src/include)

add_executable(tokenizer_test "tokenizer_test.c")
add_test(
    NAME test-tokenizer_test
    COMMAND $<TARGET_FILE:tokenizer_test>
)
target_link_libraries(tokenizer_test parser)

add_executable(rpn_test "rpn_test.c")
add_test(
    NAME test-rpn_test
    COMMAND $<TARGET_FILE:rpn_test>
)
target_link_libraries(rpn_test parser)