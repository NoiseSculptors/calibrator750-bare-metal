
cmake_minimum_required(VERSION 3.19)

add_firmware(
  NAME julia
  SRCS julia.c
  LIBS m lib::printf driver::ssd1315
)

