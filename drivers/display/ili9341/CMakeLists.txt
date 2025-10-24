
cmake_minimum_required(VERSION 3.19)
project(driver_ili9341 C)

add_library(driver_ili9341 STATIC ili9341.c)
add_library(driver::ili9341 ALIAS driver_ili9341)

target_include_directories(driver_ili9341 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(driver_ili9341 PUBLIC noisesculptors::core)

