
cmake_minimum_required(VERSION 3.16)

add_firmware(
  NAME clock_report
  SRCS clock_report.c
  LIBS lib::printf
)

