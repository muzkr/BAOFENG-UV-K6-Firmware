
# -------------------------
#   kd32_driver

add_library(kd32_driver INTERFACE)

target_include_directories(kd32_driver INTERFACE 
    CMSIS/Include
    CMSIS/Device/kd32f328xb
    StdPeriph_Driver/inc
)

target_sources(kd32_driver INTERFACE 
    StdPeriph_Driver/src/KD32f328_adc.c
    StdPeriph_Driver/src/KD32f328_crc.c
    StdPeriph_Driver/src/KD32f328_dma.c
    StdPeriph_Driver/src/KD32f328_exti.c
    StdPeriph_Driver/src/KD32f328_flash.c
    StdPeriph_Driver/src/KD32f328_gpio.c
    StdPeriph_Driver/src/KD32f328_iwdg.c
    StdPeriph_Driver/src/KD32f328_misc.c
    StdPeriph_Driver/src/KD32f328_pwr.c
    StdPeriph_Driver/src/KD32f328_rcc.c
    StdPeriph_Driver/src/KD32f328_spi.c
    StdPeriph_Driver/src/KD32f328_tim.c
    StdPeriph_Driver/src/KD32f328_usart.c
    StdPeriph_Driver/src/KD32f328_wwdg.c
)


# -------------------------
#   STM32F030x8_driver

add_library(STM32F030x8_driver INTERFACE)

target_include_directories(STM32F030x8_driver INTERFACE
    CMSIS/Include
    CMSIS/Device/STM32F0xx/Include
    STM32F0xx_HAL_Driver/Inc
)

target_compile_definitions(STM32F030x8_driver INTERFACE
    USE_FULL_LL_DRIVER
    STM32F030x8
)

target_sources(STM32F030x8_driver INTERFACE
    STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_pwr.c 
    STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_exti.c 
    STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_gpio.c 
    STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c 
    STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_utils.c 
)

# -------------------------
#   printf

add_library(printf INTERFACE)
target_include_directories(printf INTERFACE printf)
target_sources(printf INTERFACE printf/printf.c)
