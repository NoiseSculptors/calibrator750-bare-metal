
cmake_minimum_required(VERSION 3.16)
project(ext_printf C)

add_library(lib_printf OBJECT printf.c)
add_library(lib::printf ALIAS lib_printf)

target_include_directories(lib_printf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lib_printf PUBLIC noisesculptors::core)

