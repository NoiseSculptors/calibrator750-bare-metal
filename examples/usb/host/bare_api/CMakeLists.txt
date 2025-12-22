
cmake_minimum_required(VERSION 3.19)

set(APP bare_api)

add_firmware(
  NAME ${APP}
  SRCS main.c
  LIBS lib::printf
)

noisesculptors_add_tinyusb_for_app(${APP} "${CMAKE_CURRENT_SOURCE_DIR}")

