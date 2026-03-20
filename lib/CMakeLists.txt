
# -------------------------
#   kd32_driver

add_library(kd32_driver INTERFACE)

target_include_directories(kd32_driver INTERFACE 
    CMSIS/Include
    CMSIS/Device
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
#   printf

add_library(printf INTERFACE)
target_include_directories(printf INTERFACE printf)
target_sources(printf INTERFACE printf/printf.c)
