#ifndef NVIC_EXTI_H
#define NVIC_EXTI_H

/*
https://developer.arm.com/documentation/107706/0100/Exceptions-and-interrupts-overview/NVIC-registers-for-interrupt-management
--------------------------------------------------------------------------------  
   TABLE COLUMNS
1) Management function
2) Register type
3) Address
4) CMSIS-Core symbol
5) Notes
--------------------------------------------------------------------------------  
1) Enable interrupts
2) Interrupt Set Enable Registers
3) 0xE000E100 to 0xE000E13C
4) NVIC->ISER[0] to NVIC->ISER[15]
5) Write 1 to set enable
--------------------------------------------------------------------------------  
1) Disable interrupts
2) Interrupt Clear Enable Registers
3) 0xE000E180 to 0xE000E1BC
4) NVIC->ICER[0] to NVIC_ICER[15]
5) Write 1 to clear enable
--------------------------------------------------------------------------------  
1) Pend an interrupt
2) Interrupt Set Pending registers
3) 0xE000E200 to 0xE000E23C
4) NVIC->ISPR[0] to NVIC->ISPR[15]
5) Write 1 to set pending status
--------------------------------------------------------------------------------  
1) Clear a pending interrupt
2) Interrupt Clear Pending registers
3) 0xE000E280 to 0xE000E2BC
4) NVIC->ICPR[0] to NVIC->ICPR[15]
5) Write 1 to clear pending status
--------------------------------------------------------------------------------  
1) Get the interrupt status
2) Interrupt Active Bit registers
3) 0xE000E300 to 0xE000E33C
4) NVIC->IABR[0] to NVIC->IABR[15]
5) Read only Interrupt Active status bits
--------------------------------------------------------------------------------  
1) Define the priority level of an interrupt
2) Interrupt Priority Registers
3) 0xE000E400 to 0xE000E5EF
4) NVIC->IPR[0] to NVIC->IPR[495 or 123]
5) Interrupt Priority level for each external interrupt. In Armv8-M Mainline,
each IPR register contains 256 priority level (8 bits). In Armv8-M Baseline,
each IPR register contains 4 priority levels (2 bit)
--------------------------------------------------------------------------------  
1) Define the interrupt’s target security state when the Security extension is
implemented
2) Interrupt Target Non-secure State Registers
3) 0xE000E380 to 0xE000E3BC
4) NVIC->ITNS[0] to NVIC->ITNS[15]
5) Write 1 to set an interrupt to Non-secure. Clear to 0 to set an interrupt to
Secure

*/

/*

NVIC_ISER 0xE000E100
NVIC_ICER 0xE000E180
NVIC_ISPR 0xE000E200
NVIC_ICPR 0xE000E280
NVIC_IABR 0xE000E300
NVIC_IPR  0xE000E400
NVIC_ITNS 0xE000E380


#define NVIC_ENABLE(IRQn) (NVIC->ISER[(IRQn) >> 5] = (1UL << ((IRQn) & 0x1F)))

*/

#define IRQ_WWDG1                                       0U
#define IRQ_PVD_PVM                                     1U
#define IRQ_RTC_TAMP_STAMP_CSS_LSE                      2U
#define IRQ_RTC_WKUP                                    3U
#define IRQ_FLASH                                       4U
#define IRQ_RCC                                         5U
#define IRQ_EXTI0                                       6U
#define IRQ_EXTI1                                       7U
#define IRQ_EXTI2                                       8U
#define IRQ_EXTI3                                       9U
#define IRQ_EXTI4                                      10U
#define IRQ_DMA_STR0                                   11U
#define IRQ_DMA_STR1                                   12U
#define IRQ_DMA_STR2                                   13U
#define IRQ_DMA_STR3                                   14U
#define IRQ_DMA_STR4                                   15U
#define IRQ_DMA_STR5                                   16U
#define IRQ_DMA_STR6                                   17U
#define IRQ_ADC1_2                                     18U
#define IRQ_FDCAN1_IT0                                 19U
#define IRQ_FDCAN2_IT0                                 20U
#define IRQ_FDCAN1_IT1                                 21U
#define IRQ_FDCAN2_IT1                                 22U
#define IRQ_EXTI9_5                                    23U
#define IRQ_TIM1_BRK                                   24U
#define IRQ_TIM1_UP                                    25U
#define IRQ_TIM1_TRG_COM                               26U
#define IRQ_TIM_CC                                     27U
#define IRQ_TIM2                                       28U
#define IRQ_TIM3                                       29U
#define IRQ_TIM4                                       30U
#define IRQ_I2C1_EV                                    31U
#define IRQ_I2C1_ER                                    32U
#define IRQ_I2C2_EV                                    33U
#define IRQ_I2C2_ER                                    34U
#define IRQ_SPI1                                       35U
#define IRQ_SPI2                                       36U
#define IRQ_USART1                                     37U
#define IRQ_USART2                                     38U
#define IRQ_USART3                                     39U
#define IRQ_EXTI15_10                                  40U
#define IRQ_RTC_ALARM                                  41U
/* Reserved */                                        
#define IRQ_TIM8_BRK_TIM12                             43U
#define IRQ_TIM8_UP_TIM13                              44U
#define IRQ_TIM8_TRG_COM_TIM14                         45U
#define IRQ_TIM8_CC                                    46U
#define IRQ_DMA1_STR7                                  47U
#define IRQ_FMC                                        48U
#define IRQ_SDMMC1                                     49U
#define IRQ_TIM5                                       50U
#define IRQ_SPI3                                       51U
#define IRQ_UART4                                      52U
#define IRQ_UART5                                      53U
#define IRQ_TIM6_DAC                                   54U
#define IRQ_TIM7                                       55U
#define IRQ_DMA2_STR0                                  56U
#define IRQ_DMA2_STR1                                  57U
#define IRQ_DMA2_STR2                                  58U
#define IRQ_DMA2_STR3                                  59U
#define IRQ_DMA2_STR4                                  60U
#define IRQ_ETH                                        61U
#define IRQ_ETH_WKUP                                   62U
#define IRQ_FDCAN_CAL                                  63U
#define IRQ_SEND_EVEN                                  64U
/* Reserved */                                        
/* Reserved */                                        
/* Reserved */                                        
#define IRQ_DMA2_STR5                                  68U
#define IRQ_DMA2_STR6                                  69U
#define IRQ_DMA2_STR7                                  70U
#define IRQ_USART6                                     71U
#define IRQ_I2C3_EV                                    72U
#define IRQ_I2C3_ER                                    73U
#define IRQ_OTG_HS_EP1_OUT                             74U
#define IRQ_OTG_HS_EP1_IN                              75U
#define IRQ_OTG_HS_WKUP                                76U
#define IRQ_OTG_HS                                     77U
#define IRQ_DCMI                                       78U
#define IRQ_CRYP                                       79U
#define IRQ_HASH_RNG                                   80U
#define IRQ_FPU                                        81U
#define IRQ_UART7                                      82U
#define IRQ_UART8                                      83U
#define IRQ_SPI4                                       84U
#define IRQ_SPI5                                       85U
#define IRQ_SPI6                                       86U
#define IRQ_SAI1                                       87U
#define IRQ_LTDC                                       88U
#define IRQ_LTDC_ER                                    89U
#define IRQ_DMA2D                                      90U
#define IRQ_SAI2                                       91U
#define IRQ_QUADSPI                                    92U
#define IRQ_LPTIM1                                     93U
#define IRQ_CEC                                        94U
#define IRQ_I2C4_EV                                    95U
#define IRQ_I2C4_ER                                    96U
#define IRQ_SPDIF                                      97U
#define IRQ_OTG_FS_EP1_OUT                             98U
#define IRQ_OTG_FS_EP1_IN                              99U
#define IRQ_OTG_FS_WKUP                               100U
#define IRQ_OTG_FS                                    101U
#define IRQ_DMAMUX1_OV                                102U
#define IRQ_HRTIM1_MST                                103U
#define IRQ_HRTIM1_TIMA                               104U
#define IRQ_HRTIM_TIMB                                105U
#define IRQ_HRTIM1_TIMC                               106U
#define IRQ_HRTIM1_TIMD                               107U
#define IRQ_HRTIM_TIME                                108U
#define IRQ_HRTIM1_FLT                                109U
#define IRQ_DFSDM1_FLT0                               110U
#define IRQ_DFSDM1_FLT1                               111U
#define IRQ_DFSDM1_FLT2                               112U
#define IRQ_DFSDM1_FLT3                               113U
#define IRQ_SAI3                                      114U
#define IRQ_SWPMI1                                    115U
#define IRQ_TIM15                                     116U
#define IRQ_TIM16                                     117U
#define IRQ_TIM17                                     118U
#define IRQ_MDIOS_WKUP                                119U
#define IRQ_MDIOS                                     120U
#define IRQ_JPEG                                      121U
#define IRQ_MDMA                                      122U
/* Reserved */
#define IRQ_SDMMC2                                    124U
#define IRQ_HSEM0                                     125U
/* Reserved */
#define IRQ_ADC3                                      127U
#define IRQ_DMAMUX2_OVR                               128U
#define IRQ_BDMA_CH0                                  129U
#define IRQ_BDMA_CH1                                  130U
#define IRQ_BDMA_CH2                                  131U
#define IRQ_BDMA_CH3                                  132U
#define IRQ_BDMA_CH4                                  133U
#define IRQ_BDMA_CH5                                  134U
#define IRQ_BDMA_CH6                                  135U
#define IRQ_BDMA_CH7                                  136U
#define IRQ_COMP                                      137U
#define IRQ_LPTIM2                                    138U
#define IRQ_LPTIM3                                    139U
#define IRQ_LPTIM4                                    140U
#define IRQ_LPTIM5                                    141U
#define IRQ_LPUART                                    142U
#define IRQ_WWDG1_RST                                 143U
#define IRQ_CRS                                       144U
#define IRQ_RAMECC123                                 145U
#define IRQ_SAI4                                      146U
/* Reserved */
/* Reserved */
#define IRQ_WKUP                                      149U


/*
(from ST reference manual RM0433 Rev 8)
EXTI rising trigger selection register (EXTI_RTSR1) Address offset: 0x00
EXTI falling trigger selection register (EXTI_FTSR1) Address offset: 0x04
EXTI software interrupt event register (EXTI_SWIER1) Address offset: 0x08
EXTI D3 pending mask register (EXTI_D3PMR1) Address offset: 0x0C
EXTI D3 pending clear selection register low (EXTI_D3PCR1L) Address offset: 0x10
EXTI D3 pending clear selection register high (EXTI_D3PCR1H) Address offset: 0x14
EXTI rising trigger selection register (EXTI_RTSR2) Address offset: 0x20
EXTI falling trigger selection register (EXTI_FTSR2) Address offset: 0x24
EXTI software interrupt event register (EXTI_SWIER2) Address offset: 0x28
EXTI D3 pending mask register (EXTI_D3PMR2) Address offset: 0x2C
EXTI D3 pending clear selection register low (EXTI_D3PCR2L) Address offset: 0x30
EXTI D3 pending clear selection register high (EXTI_D3PCR2H) Address offset: 0x34
EXTI rising trigger selection register (EXTI_RTSR3) Address offset: 0x40
EXTI falling trigger selection register (EXTI_FTSR3) Address offset: 0x44
EXTI software interrupt event register (EXTI_SWIER3) Address offset: 0x48
EXTI D3 pending mask register (EXTI_D3PMR3) Address offset: 0x4C
EXTI D3 pending clear selection register low (EXTI_D3PCR3L) Address offset: 0x50
EXTI D3 pending clear selection register high (EXTI_D3PCR3H) Address offset: 0x54
EXTI interrupt mask register (EXTI_CPUIMR1) Address offset: 0x80
EXTI event mask register (EXTI_CPUEMR1) Address offset: 0x84
EXTI pending register (EXTI_CPUPR1) Address offset: 0x88
EXTI interrupt mask register (EXTI_CPUIMR2) Address offset: 0x90
EXTI event mask register (EXTI_CPUEMR2) Address offset: 0x94
EXTI pending register (EXTI_CPUPR2) Address offset: 0x98
EXTI interrupt mask register (EXTI_CPUIMR3) Address offset: 0xA0
EXTI event mask register (EXTI_CPUEMR3) Address offset: 0xA4
EXTI pending register (EXTI_CPUPR3) Address offset: 0xA8
*/

#define EXTI_RTSR1       (volatile unsigned int *)(EXTI + 0x00)
#define EXTI_FTSR1       (volatile unsigned int *)(EXTI + 0x04)
#define EXTI_SWIER1      (volatile unsigned int *)(EXTI + 0x08)
#define EXTI_D3PMR1      (volatile unsigned int *)(EXTI + 0x0C)
#define EXTI_D3PCR1L     (volatile unsigned int *)(EXTI + 0x10)
#define EXTI_D3PCR1H     (volatile unsigned int *)(EXTI + 0x14)
#define EXTI_RTSR2       (volatile unsigned int *)(EXTI + 0x20)
#define EXTI_FTSR2       (volatile unsigned int *)(EXTI + 0x24)
#define EXTI_SWIER2      (volatile unsigned int *)(EXTI + 0x28)
#define EXTI_D3PMR2      (volatile unsigned int *)(EXTI + 0x2C)
#define EXTI_D3PCR2L     (volatile unsigned int *)(EXTI + 0x30)
#define EXTI_D3PCR2H     (volatile unsigned int *)(EXTI + 0x34)
#define EXTI_RTSR3       (volatile unsigned int *)(EXTI + 0x40)
#define EXTI_FTSR3       (volatile unsigned int *)(EXTI + 0x44)
#define EXTI_SWIER3      (volatile unsigned int *)(EXTI + 0x48)
#define EXTI_D3PMR3      (volatile unsigned int *)(EXTI + 0x4C)
#define EXTI_D3PCR3L     (volatile unsigned int *)(EXTI + 0x50)
#define EXTI_D3PCR3H     (volatile unsigned int *)(EXTI + 0x54)
#define EXTI_CPUIMR1     (volatile unsigned int *)(EXTI + 0x80)
#define EXTI_CPUEMR1     (volatile unsigned int *)(EXTI + 0x84)
#define EXTI_CPUPR1      (volatile unsigned int *)(EXTI + 0x88)
#define EXTI_CPUIMR2     (volatile unsigned int *)(EXTI + 0x90)
#define EXTI_CPUEMR2     (volatile unsigned int *)(EXTI + 0x94)
#define EXTI_CPUPR2      (volatile unsigned int *)(EXTI + 0x98)
#define EXTI_CPUIMR3     (volatile unsigned int *)(EXTI + 0xA0)
#define EXTI_CPUEMR3     (volatile unsigned int *)(EXTI + 0xA4)
#define EXTI_CPUPR3      (volatile unsigned int *)(EXTI + 0xA8)

#endif
