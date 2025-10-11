#ifndef USART1_H
#define USART1_H

#include "init.h"

void usart1_write_char(int c);
int usart1_read_char(void);
void init_usart1(clock_info_t);

#endif
