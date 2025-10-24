
#include "user_io.h"

/* Weak defaults — any strong defs in an app-level user_io.c will override. */
#define WEAK __attribute__((weak))

WEAK void   user_io_init(void) {}
WEAK void   user_io_poll(void) {}

WEAK size_t user_btn_count(void) { return USER_NUM_BTNS; }
WEAK uint8_t user_btn_read(size_t idx) { (void)idx; return 0; }

WEAK size_t user_led_count(void) { return USER_NUM_LEDS; }
WEAK void   user_led_write(size_t idx, uint8_t on) { (void)idx; (void)on; }

WEAK size_t user_enc_count(void) { return USER_NUM_ENCS; }
WEAK user_enc_sample_t user_enc_read(size_t idx) {
    (void)idx; user_enc_sample_t s = {0,0}; return s;
}

WEAK size_t user_display_count(void) { return USER_NUM_DISPLAYS; }
WEAK void   user_display_flush(size_t idx, const void* buf, size_t bytes) {
    (void)idx; (void)buf; (void)bytes;
}

WEAK size_t user_serial_count(void) { return USER_NUM_SERIALS; }
WEAK int    user_serial_write(size_t idx, const void* d, size_t n) {
    (void)idx; (void)d; return (int)n;
}
WEAK int    user_serial_read (size_t idx, void* d, size_t n) {
    (void)idx; (void)d; (void)n; return 0;
}

