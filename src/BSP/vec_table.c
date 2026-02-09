
#include "vec_table.h"
#include <string.h>
#include "KD32f328_flash.h"
#include "Delay.h"
#include "kd32f328_it.h"
#include "PublType.h"

#define PAGE_SIZE 0x400  // 1 KB
#define FW_OFFSET 0x2000 // 8 KB

typedef void (*intHandle)(void);
static __IO intHandle UserVectors[4] __attribute__((section(".intfun")));

static const IRQn_Type IRQn_LIST[] = {
    NonMaskableInt_IRQn,
    HardFault_IRQn,
    SVC_IRQn,
    PendSV_IRQn,
    WWDG_IRQn,
    PVD_VDDIO2_IRQn,
    RTC_IRQn,
    FLASH_IRQn,
    RCC_CRS_IRQn,
    EXTI0_1_IRQn,
    EXTI2_3_IRQn,
    EXTI4_15_IRQn,
    DMA1_Channel1_IRQn,
    DMA1_Channel2_3_IRQn,
    ADC1_COMP_IRQn,
    TIM1_BRK_UP_TRG_COM_IRQn,
    TIM1_CC_IRQn,
    TIM3_IRQn,
    TIM6_DAC_IRQn,
    TIM14_IRQn,
    TIM15_IRQn,
    TIM16_IRQn,
    TIM17_IRQn,
    I2C1_IRQn,
    SPI1_IRQn,
    SPI2_IRQn,
};

static inline uint32_t IRQn_index(IRQn_Type IRQn) { return 16 + IRQn; }
static inline uint32_t vector_address(IRQn_Type IRQn) { return FLASH_BASE + 4 * IRQn_index(IRQn); }
static inline uint32_t vector_target_address(IRQn_Type IRQn) { return *((__IO uint32_t *)vector_address(IRQn)); }

static void vec_patch();
static void erase_OB();
static void erase_page();
static void program_page(const void *buf);

void vec_table_init()
{
    vec_patch();

    UserVectors[0] = SysTick_Handler;
    UserVectors[1] = USART1_IRQHandler;
    UserVectors[2] = USART2_IRQHandler;
    UserVectors[3] = DMA1_Channel4_5_IRQHandler;
}

void USART2_IRQHandler()
{
    uint32_t index = __get_IPSR() & 0x3f;
    uint32_t vec = *((__IO uint32_t *)(FLASH_BASE + FW_OFFSET + 4 * index));
    vec &= ~1u;
    ((intHandle)vec)();
}

static void vec_patch()
{
    // -----------------------------
    //  Check patching needed

    const uint32_t target = vector_target_address(USART2_IRQn);

    bool need_patch = FALSE;
    for (uint32_t i = 0; i < sizeof(IRQn_LIST) / sizeof(IRQn_Type); i++)
    {
        uint32_t target1 = vector_target_address(IRQn_LIST[i]);
        if (target1 != target)
        {
            need_patch = TRUE;
            break;
        }
    }

    if (!need_patch)
    {
        return;
    }

    // ------------------------
    // Write protection

    if (0xffffffff != FLASH->WRPR)
    {
        erase_OB();
    }

    // ------------------------
    //  Patch vector table

    uint32_t buf[PAGE_SIZE / 4];
    memcpy(buf, (void *)FLASH_BASE, sizeof(buf));

    for (uint32_t i = 0; i < sizeof(IRQn_LIST) / sizeof(IRQn_Type); i++)
    {
        buf[IRQn_index(IRQn_LIST[i])] = target;
    }

    erase_page();
    program_page(buf);
}

static void erase_page()
{
    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    FLASH_Unlock();

    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = FLASH_BASE;
    FLASH->CR |= FLASH_CR_STRT;

    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    if (SET == FLASH_GetFlagStatus(FLASH_FLAG_EOP))
    {
        FLASH_ClearFlag(FLASH_FLAG_EOP);
    }

    FLASH->CR &= ~FLASH_CR_PER;

    FLASH_Lock();
}

static void program_page(const void *buf)
{
    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    FLASH_Unlock();

    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    const uint16_t *buf1 = (const uint16_t *)buf;

    for (uint32_t size = 0; size < PAGE_SIZE; size += 2)
    {
        FLASH->CR |= FLASH_CR_PG;

        *((__IO uint16_t *)(FLASH_BASE + size)) = *buf1;
        buf1++;

        while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
        {
        }

        if (SET == FLASH_GetFlagStatus(FLASH_FLAG_EOP))
        {
            FLASH_ClearFlag(FLASH_FLAG_EOP);
        }

        FLASH->CR &= ~FLASH_CR_PG;
    }

    FLASH_Lock();
}

static void erase_OB()
{
    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    FLASH_Unlock();

    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    FLASH_OB_Unlock();

    FLASH->CR |= FLASH_CR_OPTER;
    FLASH->CR |= FLASH_CR_STRT;

    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    if (SET == FLASH_GetFlagStatus(FLASH_FLAG_EOP))
    {
        FLASH_ClearFlag(FLASH_FLAG_EOP);
    }

    FLASH->CR &= ~FLASH_CR_OPTER;

    FLASH_OB_Lock();
    FLASH_Lock();

    // ------------------
    // Load OB

    FLASH_OB_Launch();

    // Will reset automatically

    // THIS SHOULD NOT HAPPEN!
    DelayMs(10);
    NVIC_SystemReset();
    while (1)
    {
    }
}
