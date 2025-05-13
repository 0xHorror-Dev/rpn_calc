cmake_minimum_required(VERSION 3.10)

add_subdirectory("parser")
add_subdirectory("calc")

add_executable(app "source/main.cpp")

include_directories("Collections-C/src/include")
add_subdirectory("Collections-C/src")

enable_testing()

target_link_libraries(app PRIVATE parser)