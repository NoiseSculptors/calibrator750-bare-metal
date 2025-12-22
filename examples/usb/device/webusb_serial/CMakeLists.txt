
cmake_minimum_required(VERSION 3.19)

set(APP webusb_serial)

add_firmware(
  NAME ${APP}
  SRCS main.c usb_descriptors.c
  LIBS lib::printf lib::lwip
)

noisesculptors_add_tinyusb_for_app(${APP} "${CMAKE_CURRENT_SOURCE_DIR}")

