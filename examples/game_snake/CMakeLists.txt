
cmake_minimum_required(VERSION 3.19)

add_firmware(
  NAME game_snake
  SRCS game_snake.c
  LIBS lib::ugui lib::printf driver::ssd1315 driver::ssd1315_ugui
)

target_compile_definitions(game_snake PRIVATE
  USE_FONT_6X8
)

