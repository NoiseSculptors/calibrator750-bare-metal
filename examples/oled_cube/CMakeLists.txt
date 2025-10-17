
cmake_minimum_required(VERSION 3.16)

add_firmware(
  NAME oled_cube
  SRCS oled_cube.c
  LIBS driver::ssd1315
)

