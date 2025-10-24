
cmake_minimum_required(VERSION 3.19)
project(driver_ssd1315_ugui C)

add_library(driver_ssd1315_ugui STATIC ssd1315_UGUI.c)
add_library(driver::ssd1315_ugui ALIAS driver_ssd1315_ugui)

target_include_directories(driver_ssd1315_ugui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(driver_ssd1315_ugui PUBLIC lib::printf driver::ssd1315 lib::ugui)

