
cmake_minimum_required(VERSION 3.19)

add_firmware(
  NAME io 
  SRCS io.c
  LIBS lib::ugui lib::printf driver::ssd1315 driver::ssd1315_ugui
)

target_compile_definitions(io PRIVATE
  USE_FONT_8X12
)

