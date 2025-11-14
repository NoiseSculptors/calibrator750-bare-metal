
#include "user_io.h"

/* Weak defaults — any strong defs in an app-level user_io.c will override. */

__attribute__((weak)) void user_io_init(void) {}
__attribute__((weak)) void user_io_poll(void) {}
__attribute__((weak)) void user_btn_init(void) {}
__attribute__((weak)) void user_dipswitch_init(void) {}
__attribute__((weak)) void user_led_init(void) {}
__attribute__((weak)) void user_enc_init(void) {}
__attribute__((weak)) void user_display_init(void) {}
__attribute__((weak)) void user_serial_init(void) {}

__attribute__((weak)) size_t user_btn_count(void) { return USER_NUM_BTNS; }
__attribute__((weak)) uint8_t user_btn_read(size_t idx) { (void)idx; return 0; }

__attribute__((weak)) size_t user_led_count(void) { return USER_NUM_LEDS; }
__attribute__((weak)) void user_led_write(size_t idx, uint8_t on) { (void)idx; (void)on; }

__attribute__((weak)) size_t user_enc_count(void) { return USER_NUM_ENCODERS; }
__attribute__((weak)) user_enc_sample_t user_enc_read(size_t idx) {
    (void)idx; user_enc_sample_t s = {0,0,0}; return s;
}

__attribute__((weak)) size_t user_display_count(void) { return USER_NUM_DISPLAYS; }
__attribute__((weak)) void user_display_flush(size_t idx, const void* buf, size_t bytes) {
    (void)idx; (void)buf; (void)bytes;
}

__attribute__((weak)) size_t user_serial_count(void) { return USER_NUM_SERIALS; }
__attribute__((weak)) int user_serial_write(size_t idx, const void* d, size_t n) {
    (void)idx; (void)d; return (int)n;
}

__attribute__((weak)) int user_serial_read (size_t idx, void* d, size_t n) {
    (void)idx; (void)d; (void)n; return 0;
}

__attribute__((weak)) void user_serial_write_char(size_t idx, int c) {
    (void)idx; (void)c;
}

__attribute__((weak)) int user_serial_read_char(size_t idx) {
    (void)idx; return 0;
}

