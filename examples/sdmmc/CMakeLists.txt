
cmake_minimum_required(VERSION 3.19)

add_firmware(
  NAME sdmmc
  SRCS sdmmc.c
  LIBS lib::printf
)

