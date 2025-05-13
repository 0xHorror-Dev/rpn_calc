cmake_minimum_required(VERSION 3.16)

add_library(parser STATIC "src/parser.c")

target_include_directories(parser PUBLIC "inc/")
include_directories(../Collections-C/src/include)

target_link_libraries(parser PUBLIC collectc)
target_include_directories(parser PUBLIC "inc/")

add_subdirectory("tests")