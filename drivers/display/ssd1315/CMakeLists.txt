
cmake_minimum_required(VERSION 3.19)
project(driver_ssd1315 C)

add_library(driver_ssd1315 STATIC ssd1315.c)
add_library(driver::ssd1315 ALIAS driver_ssd1315)

target_include_directories(driver_ssd1315 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(driver_ssd1315 PUBLIC noisesculptors::core)

