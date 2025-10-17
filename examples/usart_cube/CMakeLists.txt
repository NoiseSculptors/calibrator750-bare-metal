
cmake_minimum_required(VERSION 3.16)

add_firmware(
  NAME usart_cube
  SRCS usart_cube.c
  LIBS lib::printf
)

