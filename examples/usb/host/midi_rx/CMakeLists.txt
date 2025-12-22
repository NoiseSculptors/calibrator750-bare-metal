
cmake_minimum_required(VERSION 3.19)

set(APP midi_rx)

add_firmware(
  NAME ${APP}
  SRCS main.c tusb_config.h
)

noisesculptors_add_tinyusb_for_app(${APP} "${CMAKE_CURRENT_SOURCE_DIR}")

