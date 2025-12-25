
cmake_minimum_required(VERSION 3.19)

add_subdirectory(blink)
add_subdirectory(clock_report)
add_subdirectory(dma_i2c_oled)
add_subdirectory(game_snake)
add_subdirectory(io)
add_subdirectory(julia)

# TODO
#add_subdirectory(lcd)

add_subdirectory(mco)
add_subdirectory(oled_cube)
add_subdirectory(oled_uGUI_hello_world)
add_subdirectory(qspi)
add_subdirectory(reset)

# TODO
#add_subdirectory(sdmmc)

add_subdirectory(systick)
add_subdirectory(timer_generated)
add_subdirectory(timer_tui)
add_subdirectory(usart_cube)
add_subdirectory(usb/device/audio_4_channel_mic)
add_subdirectory(usb/device/audio_test)
add_subdirectory(usb/device/audio_test_multi_rate)
add_subdirectory(usb/device/cdc_dual_ports)
add_subdirectory(usb/device/cdc_msc)
add_subdirectory(usb/device/cdc_uac2)
add_subdirectory(usb/device/dfu)
add_subdirectory(usb/device/dfu_runtime)
add_subdirectory(usb/device/dynamic_configuration)
add_subdirectory(usb/device/hid_boot_interface)
add_subdirectory(usb/device/hid_composite)
add_subdirectory(usb/device/hid_generic_inout)
add_subdirectory(usb/device/hid_multiple_interface)
add_subdirectory(usb/device/midi_test)
add_subdirectory(usb/device/msc_dual_lun)
add_subdirectory(usb/device/mtp)
add_subdirectory(usb/device/net_lwip_webserver)
add_subdirectory(usb/device/uac2_headset)
add_subdirectory(usb/device/uac2_speaker_fb)
add_subdirectory(usb/device/usbtmc)
add_subdirectory(usb/device/video_capture)
add_subdirectory(usb/device/video_capture_2ch)
add_subdirectory(usb/device/webusb_serial)
add_subdirectory(usb/host/bare_api)

# TODO: host mode
#add_subdirectory(usb/host/cdc_msc_hid)
#add_subdirectory(usb/host/device_info)
#add_subdirectory(usb/host/hid_controller)
#add_subdirectory(usb/host/midi_rx)
##add_subdirectory(usb/host/msc_file_explorer)

