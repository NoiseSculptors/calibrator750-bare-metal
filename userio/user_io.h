
#ifndef USER_IO_H
#define USER_IO_H

#include <stdint.h>
#include <stddef.h>

#ifdef USER_IO_HAVE_CONFIG
#  include "user_io_config.h"
#else
#  include "user_io_config_default.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Lifecycle ---- */
void   user_io_init(void);
void   user_btn_init(void);
void   user_dipswitch_init(void);
void   user_led_init(void);
void   user_enc_init(void);
void   user_display_init(void);
void   user_serial_init(void);

void   user_io_poll(void);

/* ---- Buttons ---- */
size_t user_btn_count(void);
uint8_t user_btn_read(size_t idx); /* return 0/1 */

/* ---- DIP switches---- */
size_t user_dipswitch_count(void);
uint32_t user_dipswitch_read(size_t idx);

/* ---- LEDs ---- */
size_t user_led_count(void);
void   user_led_write(size_t idx, uint8_t on);
void   user_led_toggle(size_t idx);

/* ---- Encoders ---- */
typedef struct { uint16_t value;
                 int16_t delta;
                 uint8_t pushed; } user_enc_sample_t;
size_t user_enc_count(void);
user_enc_sample_t user_enc_read(size_t idx);

/* ---- Displays ---- */
size_t user_display_count(void);
void   user_display_flush(size_t idx, const void* buf, size_t bytes);

/* ---- Serial/UART ---- */
size_t user_serial_count(void);
int    user_serial_write(size_t idx, const void* data, size_t len);
int    user_serial_read (size_t idx, void* data, size_t maxlen);
void   user_serial_write_char(size_t idx, int c);
int    user_serial_read_char(size_t idx);
void   hexdump(uint32_t addr, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* USER_IO_H */

