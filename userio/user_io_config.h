
#ifndef USER_IO_CONFIG_h
#define USER_IO_CONFIG_h

#define USER_NUM_BTNS          2
#define USER_NUM_LEDS          2
#define USER_NUM_DIP_SWITCHES  1
#define USER_NUM_ENCS          1
#define USER_NUM_DISPLAYS      1
#define USER_NUM_SERIALS       1

/* user_io_config.h  — pick exactly ONE mapping.
   New users: uncomment ONE line below that matches your board.
*/

// Used on Rocket Calibrator prototype
// #define USART3_PB11_RX_PB10_TX

// Pick one:
#define USART1_PA10_RX_PA9_TX
// #define USART1_PB15_RX_PB14_TX
// #define USART1_PB7_RX_PB6_TX
// #define USART2_PA3_RX_PA2_TX
// #define USART2_PD6_RX_PD5_TX
// #define USART3_PB11_RX_PB10_TX
// #define USART3_PC11_RX_PC10_TX
// #define USART3_PD9_RX_PD8_TX
// #define USART6_PC7_RX_PC6_TX
// #define LPUART1_PA10_RX_PA9_TX
// #define LPUART1_PB7_RX_PB6_TX
// #define UART4_PA1_RX_PA0_TX
// #define UART4_PB8_RX_PB9_TX
// #define UART4_PC11_RX_PC10_TX
// #define UART4_PD0_RX_PD1_TX
// #define UART5_PB12_RX_PB13_TX
// #define UART5_PB5_RX_PB6_TX
// #define UART5_PD2_RX_PC12_TX
// #define UART7_PB3_RX_PB4_TX
// #define UART7_PE7_RX_PE8_TX
// #define UART7_PA8_RX_PA15_TX
// #define UART8_PE0_RX_PE1_TX

/* Safety: exactly one must be defined */
#if ( \
  defined(LPUART1_PA10_RX_PA9_TX) + \
  defined(LPUART1_PB7_RX_PB6_TX) + \
  defined(UART4_PA1_RX_PA0_TX)   + \
  defined(UART4_PB8_RX_PB9_TX)   + \
  defined(UART4_PC11_RX_PC10_TX) + \
  defined(UART4_PD0_RX_PD1_TX)   + \
  defined(UART5_PB12_RX_PB13_TX) + \
  defined(UART5_PB5_RX_PB6_TX)   + \
  defined(UART5_PD2_RX_PC12_TX)  + \
  defined(UART7_PB3_RX_PB4_TX)   + \
  defined(UART7_PE7_RX_PE8_TX)   + \
  defined(UART7_PA8_RX_PA15_TX)  + \
  defined(UART8_PE0_RX_PE1_TX)   + \
  defined(USART1_PA10_RX_PA9_TX) + \
  defined(USART1_PB15_RX_PB14_TX)+ \
  defined(USART1_PB7_RX_PB6_TX)  + \
  defined(USART2_PA3_RX_PA2_TX)  + \
  defined(USART2_PD6_RX_PD5_TX)  + \
  defined(USART3_PB11_RX_PB10_TX)+ \
  defined(USART3_PC11_RX_PC10_TX)+ \
  defined(USART3_PD9_RX_PD8_TX)  + \
  defined(USART6_PC7_RX_PC6_TX) ) != 1
  #error "In user_io_config.h: define exactly ONE UART mapping macro."
#endif

#endif

