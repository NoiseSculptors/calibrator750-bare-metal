
#include <stdint.h>
#include <stddef.h>
#include "printf.h"
#include "user_io.h"

void hexdump(uint32_t addr, size_t len)
{
    const uint8_t *p = (const uint8_t *)addr;
    char ascii[17];
    ascii[16] = '\0';

    for (size_t i = 0; i < len; i += 16) {
        size_t line_len = (len - i < 16) ? (len - i) : 16;

        // offset
        printf("%08lX  ", (unsigned long)(addr + i));

        // hex bytes + build ascii
        for (size_t j = 0; j < 16; ++j) {
            if (j < line_len) {
                uint8_t c = p[i + j];
                printf("%02X ", c);
                ascii[j] = (c >= 32 && c < 127) ? (char)c : '.';
            } else {
                printf("   ");
                ascii[j] = ' ';
            }
            if (j == 7) {
                printf(" ");
            }
        }

        ascii[16] = '\0';
        printf(" |%s|\n", ascii);
    }
}

