#ifndef BOARD_API_H
#define BOARD_API_H

#include <stdint.h>
#include <stddef.h>

void main_tinyusb(void);
void board_init(void);
void board_init_after_tusb(void);
uint32_t board_millis(void);
void board_led_write(int);
size_t board_get_unique_id(uint8_t id[], size_t max_len);
size_t board_usb_get_serial(uint16_t desc_str1[], size_t max_chars);

#endif
