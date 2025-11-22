
cmake_minimum_required(VERSION 3.19)

add_firmware(
  NAME qspi
  SRCS qspi.c
  LIBS lib::printf
)

