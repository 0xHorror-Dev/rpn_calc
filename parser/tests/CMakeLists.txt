

include_directories(../Collections-C/src/include)

add_executable(pt1 "t1.c")
add_test(
    NAME test-pt1
    COMMAND $<TARGET_FILE:pt1>
)
target_link_libraries(pt1 parser)

add_executable(pt2 "t2.c")
add_test(
    NAME test-pt2
    COMMAND $<TARGET_FILE:pt2>
)
target_link_libraries(pt2 parser)