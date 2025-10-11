
#include <stdint.h>
#include "vtor.h"

extern uint32_t __StackTop;
extern uint32_t __VectorTable;

extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

void def_irq_handler(void);
void Reset_Handler(void);

/* Core/IRQ weak aliases (unchanged) */
void NMI_Handler(void)                         __attribute__ ((weak, alias("def_irq_handler")));
void HardFault_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void MemManage_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void BusFault_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void UsageFault_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void SVC_Handler(void)                         __attribute__ ((weak, alias("def_irq_handler")));
void DebugMon_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void PendSV_Handler(void)                      __attribute__ ((weak, alias("def_irq_handler")));
void SysTick_Handler(void)                     __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_WWDG1_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_PVD_PVM_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_RTC_TAMP_STAMP_CSS_LSE_Handler(void)  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_RTC_WKUP_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_FLASH_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_RCC_Handler(void)                     __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_EXTI0_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_EXTI1_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_EXTI2_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_EXTI3_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_EXTI4_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA_STR0_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA_STR1_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA_STR2_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA_STR3_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA_STR4_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA_STR5_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA_STR6_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_ADC1_2_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_FDCAN1_IT0_Handler(void)              __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_FDCAN2_IT0_Handler(void)              __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_FDCAN1_IT1_Handler(void)              __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_FDCAN2_IT1_Handler(void)              __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_EXTI9_5_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM1_BRK_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM1_UP_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM1_TRG_COM_Handler(void)            __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM_CC_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM2_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM3_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM4_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_I2C1_EV_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_I2C1_ER_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_I2C2_EV_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_I2C2_ER_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SPI1_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SPI2_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_USART1_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_USART2_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_USART3_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_EXTI15_10_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_RTC_ALARM_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM8_BRK_TIM12_Handler(void)          __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM8_UP_TIM13_Handler(void)           __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM8_TRG_COM_TIM14_Handler(void)      __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM8_CC_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA1_STR7_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_FMC_Handler(void)                     __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SDMMC1_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM5_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SPI3_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_UART4_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_UART5_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM6_DAC_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM7_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA2_STR0_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA2_STR1_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA2_STR2_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA2_STR3_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA2_STR4_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_ETH_Handler(void)                     __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_ETH_WKUP_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_FDCAN_CAL_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SEND_EVEN_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA2_STR5_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA2_STR6_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA2_STR7_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_USART6_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_I2C3_EV_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_I2C3_ER_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_OTG_HS_EP1_OUT_Handler(void)          __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_OTG_HS_EP1_IN_Handler(void)           __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_OTG_HS_WKUP_Handler(void)             __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_OTG_HS_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DCMI_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_CRYP_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_HASH_RNG_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_FPU_Handler(void)                     __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_UART7_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_UART8_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SPI4_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SPI5_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SPI6_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SAI1_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_LTDC_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_LTDC_ER_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMA2D_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SAI2_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_QUADSPI_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_LPTIM1_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_CEC_Handler(void)                     __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_I2C4_EV_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_I2C4_ER_Handler(void)                 __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SPDIF_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_OTG_FS_EP1_OUT_Handler(void)          __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_OTG_FS_EP1_IN_Handler(void)           __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_OTG_FS_WKUP_Handler(void)             __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_OTG_FS_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMAMUX1_OV_Handler(void)              __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_HRTIM1_MST_Handler(void)              __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_HRTIM1_TIMA_Handler(void)             __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_HRTIM_TIMB_Handler(void)              __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_HRTIM1_TIMC_Handler(void)             __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_HRTIM1_TIMD_Handler(void)             __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_HRTIM_TIME_Handler(void)              __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_HRTIM1_FLT_Handler(void)              __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DFSDM1_FLT0_Handler(void)             __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DFSDM1_FLT1_Handler(void)             __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DFSDM1_FLT2_Handler(void)             __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DFSDM1_FLT3_Handler(void)             __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SAI3_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SWPMI1_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM15_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM16_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_TIM17_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_MDIOS_WKUP_Handler(void)              __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_MDIOS_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_JPEG_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_MDMA_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SDMMC2_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_HSEM0_Handler(void)                   __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_ADC3_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_DMAMUX2_OVR_Handler(void)             __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_BDMA_CH0_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_BDMA_CH1_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_BDMA_CH2_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_BDMA_CH3_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_BDMA_CH4_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_BDMA_CH5_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_BDMA_CH6_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_BDMA_CH7_Handler(void)                __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_COMP_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_LPTIM2_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_LPTIM3_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_LPTIM4_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_LPTIM5_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_LPUART_Handler(void)                  __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_WWDG1_RST_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_CRS_Handler(void)                     __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_RAMECC123_Handler(void)               __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_SAI4_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));
void IRQ_WKUP_Handler(void)                    __attribute__ ((weak, alias("def_irq_handler")));

int main(void);

/* Default handler to catch unimplemented IRQs */
void def_irq_handler(void) { while (1); }

void Reset_Handler(void) {
    /* Program VTOR to point at the AXI-resident vector table.
       (Vector table itself is linked into .isr_vector at 0x24000000.) */
    *SCB_VTOR = (uint32_t)&__VectorTable;
    __asm volatile ("dsb");
    __asm volatile ("isb");

    /* Enable CP10/CP11 (FPU) */
    #define SCB_CPACR ((volatile uint32_t*)0xE000ED88)
    *SCB_CPACR |= (0xFu << 20);
    __asm volatile ("dsb");
    __asm volatile ("isb");

    /* Because we run from SRAM and .data is placed in AXI with no LMA,
       there is nothing to copy for .data. We ONLY clear .bss. */
    uint32_t *dst = &__bss_start__;
    while (dst < &__bss_end__) {
        *dst++ = 0;
    }

    /* Jump to main */
    (void)main();

    /* If main returns, trap */
    while (1) {;}
}

/* Vector table — placed at start of AXI SRAM by the linker script */
__attribute__((section(".isr_vector")))
__attribute__((used, aligned(128)))
void (* const vector_table[])(void) = {
    (void (*)(void))(&__StackTop), /* Initial MSP (address value) */
    Reset_Handler,                 /* Reset Handler */
    NMI_Handler,                   /* NMI Handler */
    HardFault_Handler,             /* Hard Fault Handler */
    MemManage_Handler,             /* MPU Fault Handler */
    BusFault_Handler,              /* Bus Fault Handler */
    UsageFault_Handler,            /* Usage Fault Handler */
    0,                             /* Reserved */
    0,                             /* Reserved */
    0,                             /* Reserved */
    0,                             /* Reserved */
    SVC_Handler,                   /* SVCall Handler */
    DebugMon_Handler,              /* Debug Monitor Handler */
    0,                             /* Reserved */
    PendSV_Handler,                /* PendSV Handler */
    SysTick_Handler,               /* SysTick Handler */

    /* External IRQs — your list follows unchanged */
    IRQ_WWDG1_Handler,
    IRQ_PVD_PVM_Handler,
    IRQ_RTC_TAMP_STAMP_CSS_LSE_Handler,
    IRQ_RTC_WKUP_Handler,
    IRQ_FLASH_Handler,
    IRQ_RCC_Handler,
    IRQ_EXTI0_Handler,
    IRQ_EXTI1_Handler,
    IRQ_EXTI2_Handler,
    IRQ_EXTI3_Handler,
    IRQ_EXTI4_Handler,
    IRQ_DMA_STR0_Handler,
    IRQ_DMA_STR1_Handler,
    IRQ_DMA_STR2_Handler,
    IRQ_DMA_STR3_Handler,
    IRQ_DMA_STR4_Handler,
    IRQ_DMA_STR5_Handler,
    IRQ_DMA_STR6_Handler,
    IRQ_ADC1_2_Handler,
    IRQ_FDCAN1_IT0_Handler,
    IRQ_FDCAN2_IT0_Handler,
    IRQ_FDCAN1_IT1_Handler,
    IRQ_FDCAN2_IT1_Handler,
    IRQ_EXTI9_5_Handler,
    IRQ_TIM1_BRK_Handler,
    IRQ_TIM1_UP_Handler,
    IRQ_TIM1_TRG_COM_Handler,
    IRQ_TIM_CC_Handler,
    IRQ_TIM2_Handler,
    IRQ_TIM3_Handler,
    IRQ_TIM4_Handler,
    IRQ_I2C1_EV_Handler,
    IRQ_I2C1_ER_Handler,
    IRQ_I2C2_EV_Handler,
    IRQ_I2C2_ER_Handler,
    IRQ_SPI1_Handler,
    IRQ_SPI2_Handler,
    IRQ_USART1_Handler,
    IRQ_USART2_Handler,
    IRQ_USART3_Handler,
    IRQ_EXTI15_10_Handler,
    IRQ_RTC_ALARM_Handler,
    0, /* Reserved */
    IRQ_TIM8_BRK_TIM12_Handler,
    IRQ_TIM8_UP_TIM13_Handler,
    IRQ_TIM8_TRG_COM_TIM14_Handler,
    IRQ_TIM8_CC_Handler,
    IRQ_DMA1_STR7_Handler,
    IRQ_FMC_Handler,
    IRQ_SDMMC1_Handler,
    IRQ_TIM5_Handler,
    IRQ_SPI3_Handler,
    IRQ_UART4_Handler,
    IRQ_UART5_Handler,
    IRQ_TIM6_DAC_Handler,
    IRQ_TIM7_Handler,
    IRQ_DMA2_STR0_Handler,
    IRQ_DMA2_STR1_Handler,
    IRQ_DMA2_STR2_Handler,
    IRQ_DMA2_STR3_Handler,
    IRQ_DMA2_STR4_Handler,
    IRQ_ETH_Handler,
    IRQ_ETH_WKUP_Handler,
    IRQ_FDCAN_CAL_Handler,
    IRQ_SEND_EVEN_Handler,
    0, 0, 0, /* Reserved */
    IRQ_DMA2_STR5_Handler,
    IRQ_DMA2_STR6_Handler,
    IRQ_DMA2_STR7_Handler,
    IRQ_USART6_Handler,
    IRQ_I2C3_EV_Handler,
    IRQ_I2C3_ER_Handler,
    IRQ_OTG_HS_EP1_OUT_Handler,
    IRQ_OTG_HS_EP1_IN_Handler,
    IRQ_OTG_HS_WKUP_Handler,
    IRQ_OTG_HS_Handler,
    IRQ_DCMI_Handler,
    IRQ_CRYP_Handler,
    IRQ_HASH_RNG_Handler,
    IRQ_FPU_Handler,
    IRQ_UART7_Handler,
    IRQ_UART8_Handler,
    IRQ_SPI4_Handler,
    IRQ_SPI5_Handler,
    IRQ_SPI6_Handler,
    IRQ_SAI1_Handler,
    IRQ_LTDC_Handler,
    IRQ_LTDC_ER_Handler,
    IRQ_DMA2D_Handler,
    IRQ_SAI2_Handler,
    IRQ_QUADSPI_Handler,
    IRQ_LPTIM1_Handler,
    IRQ_CEC_Handler,
    IRQ_I2C4_EV_Handler,
    IRQ_I2C4_ER_Handler,
    IRQ_SPDIF_Handler,
    IRQ_OTG_FS_EP1_OUT_Handler,
    IRQ_OTG_FS_EP1_IN_Handler,
    IRQ_OTG_FS_WKUP_Handler,
    IRQ_OTG_FS_Handler,
    IRQ_DMAMUX1_OV_Handler,
    IRQ_HRTIM1_MST_Handler,
    IRQ_HRTIM1_TIMA_Handler,
    IRQ_HRTIM_TIMB_Handler,
    IRQ_HRTIM1_TIMC_Handler,
    IRQ_HRTIM1_TIMD_Handler,
    IRQ_HRTIM_TIME_Handler,
    IRQ_HRTIM1_FLT_Handler,
    IRQ_DFSDM1_FLT0_Handler,
    IRQ_DFSDM1_FLT1_Handler,
    IRQ_DFSDM1_FLT2_Handler,
    IRQ_DFSDM1_FLT3_Handler,
    IRQ_SAI3_Handler,
    IRQ_SWPMI1_Handler,
    IRQ_TIM15_Handler,
    IRQ_TIM16_Handler,
    IRQ_TIM17_Handler,
    IRQ_MDIOS_WKUP_Handler,
    IRQ_MDIOS_Handler,
    IRQ_JPEG_Handler,
    IRQ_MDMA_Handler,
    0, /* Reserved */
    IRQ_SDMMC2_Handler,
    IRQ_HSEM0_Handler,
    0, /* Reserved */
    IRQ_ADC3_Handler,
    IRQ_DMAMUX2_OVR_Handler,
    IRQ_BDMA_CH0_Handler,
    IRQ_BDMA_CH1_Handler,
    IRQ_BDMA_CH2_Handler,
    IRQ_BDMA_CH3_Handler,
    IRQ_BDMA_CH4_Handler,
    IRQ_BDMA_CH5_Handler,
    IRQ_BDMA_CH6_Handler,
    IRQ_BDMA_CH7_Handler,
    IRQ_COMP_Handler,
    IRQ_LPTIM2_Handler,
    IRQ_LPTIM3_Handler,
    IRQ_LPTIM4_Handler,
    IRQ_LPTIM5_Handler,
    IRQ_LPUART_Handler,
    IRQ_WWDG1_RST_Handler,
    IRQ_CRS_Handler,
    IRQ_RAMECC123_Handler,
    IRQ_SAI4_Handler,
    0, 0, /* Reserved */
    IRQ_WKUP_Handler
};

