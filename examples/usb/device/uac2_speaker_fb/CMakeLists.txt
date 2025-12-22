
cmake_minimum_required(VERSION 3.19)

set(APP uac2_speaker_fb)

add_firmware(
  NAME ${APP}
  SRCS main.c usb_descriptors.c
  LIBS lib::printf
)

target_compile_definitions(${APP}
    PRIVATE CFG_AUDIO_DEBUG=1
            CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_RX=2
)

noisesculptors_add_tinyusb_for_app(${APP} "${CMAKE_CURRENT_SOURCE_DIR}")

