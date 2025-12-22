
cmake_minimum_required(VERSION 3.19)

set(APP mtp)

add_firmware(
  NAME ${APP}
  SRCS main.c mtp_fs_example.c usb_descriptors.c usb_mtp.c
  LIBS lib::printf
)

noisesculptors_add_tinyusb_for_app(${APP} "${CMAKE_CURRENT_SOURCE_DIR}")

