
cmake_minimum_required(VERSION 3.19)

add_firmware(
  NAME usb_mtp
  SRCS
       usb_mtp.c
       main.c
       mtp_fs_example.c
       usb_descriptors.c
  LIBS lib::printf
)

noisesculptors_add_tinyusb_for_app(usb_mtp "${CMAKE_CURRENT_SOURCE_DIR}")

