#ifndef BOARD_API_H
#define BOARD_API_H

#include <stdint.h>
#include <stddef.h>

size_t board_get_unique_id(uint8_t id[], size_t max_len);
size_t board_usb_get_serial(uint16_t desc_str1[], size_t max_chars);
uint32_t board_button_read(void);
uint32_t board_millis(void);
void board_delay(uint32_t ms);
void board_init(void);
void board_init_after_tusb(void);
void board_led_write(int);
void board_reset_to_bootloader(void);
void main_tinyusb(void);

#endif
