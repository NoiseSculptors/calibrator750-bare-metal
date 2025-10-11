#ifndef SYSTICK_h
#define SYSTICK_h

#define SYSTICK_CTRL    (volatile unsigned int *)(0xE000E010)
#define SYSTICK_LOAD    (volatile unsigned int *)(0xE000E014)
#define SYSTICK_VAL     (volatile unsigned int *)(0xE000E018)
#define SYSTICK_CALIB   (volatile unsigned int *)(0xE000E01C)

#endif
