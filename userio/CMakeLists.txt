
cmake_minimum_required(VERSION 3.19)
project(app_userio C)

add_library(app_userio STATIC serial.c user_io_default.c)
add_library(lib::userio ALIAS app_userio)

target_link_libraries(app_userio PUBLIC lib::printf)

target_include_directories(app_userio PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

if(EXISTS "${PROJECT_SOURCE_DIR}/user_io_config.h")
  target_include_directories(app_userio PUBLIC "${PROJECT_SOURCE_DIR}")
  target_compile_definitions(app_userio PUBLIC USER_IO_HAVE_CONFIG=1)
endif()

target_compile_options(app_userio PRIVATE -ffunction-sections -fdata-sections)

