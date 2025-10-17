
cmake_minimum_required(VERSION 3.16)

add_firmware(
  NAME timer_tui
  SRCS timer_tui.c
  LIBS lib::printf
)

