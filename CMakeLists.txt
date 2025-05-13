cmake_minimum_required(VERSION 3.10)

project(rpn_calc C)

enable_testing()

add_subdirectory("parser")
add_subdirectory("calc")

option(RPN_BUILD_GUI "build gtk4 gui application" ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_EXTENSIONS OFF)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Werror)
    
    add_compile_options(-Wall -Wextra)
    # More specific warnings can be added as needed
    add_compile_options(-Wconversion -Wshadow -Wundef -Wunused)
    add_compile_options(-Wno-unsafe-buffer-usage -Wno-unused-macros)
endif()

if(RPN_BUILD_GUI)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
	add_executable(app "source/main.c")
endif(RPN_BUILD_GUI)

if(MSVC)
    add_compile_options(/W4 /WX)  
endif()

include_directories("Collections-C/src/include")
add_subdirectory("Collections-C/src")

if(RPN_BUILD_GUI)
	target_link_libraries(app PRIVATE parser calc PkgConfig::GTK4)
endif(RPN_BUILD_GUI)