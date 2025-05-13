cmake_minimum_required(VERSION 3.10)

add_subdirectory("parser")
add_subdirectory("calc")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)


add_executable(app "source/main.c")

include_directories("Collections-C/src/include")
add_subdirectory("Collections-C/src")

enable_testing()

target_link_libraries(app PRIVATE parser calc PkgConfig::GTK4)