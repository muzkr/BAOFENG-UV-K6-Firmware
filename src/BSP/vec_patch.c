
#include "vec_patch.h"
#include <string.h>
#include "printf.h"
#include "KD32f328_flash.h"
#include "Delay.h"

static const IRQn_Type IRQn_array[] = {
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
static inline uint32_t vector_target_address(IRQn_Type IRQn) { return *((uint32_t *)vector_address(IRQn)); }

static void erase_OB();
static void erase_page();
static void program_page(const uint32_t *buf);

void vec_patch()
{

    printf("---------------\n");
    printf("patching vector table..\n");
    printf("pause for 5 seconds..\n");

    DelayMs(5000);

    printf("---------------\n");
    printf("patching vector table..\n");

    // -----------------------------
    //  Check patching needed

    const uint32_t target = vector_target_address(USART2_IRQn);
    printf("target (USART2_IRQn) address: %08x\n", target);

    //
    bool need_patch = FALSE;
    for (uint32_t i = 0; i < sizeof(IRQn_array) / sizeof(IRQn_Type); i++)
    {
        uint32_t target1 = vector_target_address(IRQn_array[i]);
        if (target1 == target)
        {
            printf("IRQ %d: %08x (OK)\n", IRQn_array[i], target1);
        }
        else
        {
            printf("IRQ %d: %08x (need patch!)\n", IRQn_array[i], target1);
            need_patch = TRUE;
        }
    }

    if (!need_patch)
    {
        printf("all good. \n");
        return;
    }

    // ------------------------
    // write protection

    uint32_t wp = FLASH->WRPR;
    printf("write protection: %08x\n", wp);
    if (0xffffffff == wp)
    {
        printf("not protected\n");
    }
    else
    {
        printf("protected\n");
        printf("erasing option bytes to disable write protection..\n");
        erase_OB();
    }

    // ------------------------
    //  patch vector table

    uint32_t buf[1024 / 4];
    memcpy(buf, (void *)FLASH_BASE, sizeof(buf));

    for (uint32_t i = 0; i < sizeof(IRQn_array) / sizeof(IRQn_Type); i++)
    {
        buf[IRQn_index(IRQn_array[i])] = target;
    }

    printf("erasing page..\n");
    erase_page();

    printf("programming page..\n");
    program_page(buf);

    printf("done\n");
    printf("reset now\n");
}

static void erase_page()
{

    FLASH_Unlock();

    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    printf("start\n");

    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = FLASH_BASE;
    FLASH->CR |= FLASH_CR_STRT;

    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    printf("finish\n");

    if (SET == FLASH_GetFlagStatus(FLASH_FLAG_EOP))
    {
        FLASH_ClearFlag(FLASH_FLAG_EOP);
    }

    FLASH->CR &= ~FLASH_CR_PER;

    FLASH_Lock();
}

static void program_page(const uint32_t *buf)
{
    FLASH_Unlock();

    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    printf("start\n");

    const uint16_t *buf1 = (const uint16_t *)buf;

    for (uint32_t size = 0; size < 1024; size += 2)
    {
        printf("%d .. ", size);

        FLASH->CR |= FLASH_CR_PG;
        *(__IO uint16_t *)(FLASH_BASE + size) = *buf1;
        buf1++;

        while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
        {
        }

        printf("OK\n");

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
    printf("erasing opt bytes.. \n");

    FLASH_Unlock();

    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    printf("start\n");

    FLASH_OB_Unlock();

    FLASH->CR |= FLASH_CR_OPTER;
    FLASH->CR |= FLASH_CR_STRT;

    while (SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY))
    {
    }

    printf("finished\n");

    if (SET == FLASH_GetFlagStatus(FLASH_FLAG_EOP))
    {
        FLASH_ClearFlag(FLASH_FLAG_EOP);
    }

    FLASH->CR &= ~FLASH_CR_OPTER;

    FLASH_OB_Lock();
    FLASH_Lock();

    printf("loading option bytes..\n");
    printf("should reset\n");

    FLASH_OB_Launch();

    while (1)
    {
        printf("THIS SHOULD NOT HAPPEN!\n");
        DelayMs(1000);
    }
}
