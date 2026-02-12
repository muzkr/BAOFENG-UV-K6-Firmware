#include "includes.h"

/***************************   IO 接口  *****************************/
#define LCD_CS_H GPIOB->BSRR = GPIO_Pin_12
#define LCD_CS_L GPIOB->BRR = GPIO_Pin_12

#define LCD_RS_H GPIOC->BSRR = GPIO_Pin_15
#define LCD_RS_L GPIOC->BRR = GPIO_Pin_15

#define LCD_RST_H GPIOB->BSRR = GPIO_Pin_8
#define LCD_RST_L GPIOB->BRR = GPIO_Pin_8
/*********************************************************************/
U8 gLcdBuffer[8][128] __attribute__((section(".bss.LARGE")));
/*********************************************************************/

// In ~120 ns
void SC5260_delay(U32 i)
{
    const register U32 f = SystemCoreClock;
    while (i--)
    {
        if (f > 48000000)
        {
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
        }
    }
}

void SC5260_writeCmd(U8 dat)
{
    LCD_CS_L;
    LCD_RS_L;
    SC5260_delay(1);

    while (RESET == SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE))
        ;
    SPI_SendData8(SPI2, dat);
    while (SET == SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY))
        ;
    SC5260_delay(1);
    LCD_CS_H;
    SC5260_delay(1);
}

void SC5260_writeDat(U8 dat)
{
    while (RESET == SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE))
        ;
    SPI_SendData8(SPI2, dat);
    while (SET == SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY))
        ;
}

void SC5260_SetStartPosition(U8 Line, U8 column)
{
    SC5260_writeCmd(0xB0 | (Line & 0x0f));
    SC5260_writeCmd(0x10 | ((column >> 4) & 0x0f));
    SC5260_writeCmd(column & 0x0f);
}

extern void SC5260_FillScreen(U8 value)
{
    int i, j;

    for (i = 0; i < 8; i++)
    {
        SC5260_SetStartPosition(i, 0);

        LCD_CS_L;
        LCD_RS_H;
        SC5260_delay(1);
        for (j = 0; j < 132; j++)
        {
            SC5260_writeDat(value);
        }
    }
    SC5260_delay(1);
    LCD_CS_H;
}

extern void SC5260_Init(void)
{
    LCD_RST_L;
    DelayMs(10);
    LCD_RST_H;

    DelayMs(100);
    SC5260_writeCmd(0xE2);
    DelayMs(100);

    SC5260_writeCmd(0x2c);
    DelayMs(10);
    SC5260_writeCmd(0x2e);
    DelayMs(10);
    SC5260_writeCmd(0x2f);
    DelayMs(10);

    SC5260_writeCmd(0x23);
    SC5260_writeCmd(0x81);
    SC5260_writeCmd(0x26);
    SC5260_writeCmd(0xA2);
    SC5260_writeCmd(0xA1);
    SC5260_writeCmd(0xC0);
    SC5260_writeCmd(0x40);
    SC5260_writeCmd(0xAE);

    SC5260_FillScreen(0x00);

    SC5260_writeCmd(0xAF);
}

// 设置液晶对比度
extern void SC5620_SetContrastRatio(U8 level)
{
    const U8 setLevel[5] = {37, 41, 45, 48, 51};

    if (level > 4)
    {
        level = 4;
    }

    SC5260_writeCmd(0x81);
    SC5260_writeCmd(setLevel[level]); // 对比度细调
}

void SC5260_DisplaySmallArea(U8 posY, U8 posX, U8 width, U8 height, const U8 *pdat, U8 flagClear, U8 flagInvert)
{
    if (height == 0 || width == 0)
    {
        return;
    }

    const U8 curPage = posY / 8;
    const U8 page_off = posY % 8;
    const U8 page_rem = 8 - page_off;

    U8 preserve_mask;
    U8 src_mask;
    if (height < page_rem)
    {
        preserve_mask = ((1 << page_off) - 1)                                       //
                        | (((1 << (page_rem - height)) - 1) << (page_off + height)) //
            ;
        src_mask = (1 << height) - 1;
    }
    else
    {
        preserve_mask = (1 << page_off) - 1;
        src_mask = (1 << page_rem) - 1;
    }

    const bool span_pages = height > page_rem && curPage < 7;
    U8 preserve_mask1 = 0;
    if (span_pages)
    {
        const U8 span_height = height - page_rem;
        preserve_mask1 = ((1 << (8 - span_height)) - 1) << span_height;
    }

    for (uint32_t i = 0; i < width; i++)
    {
        const U8 posX1 = posX + i;
        if (posX1 >= 128)
        {
            break;
        }

        uint8_t src_bits = flagClear ? 0 : pdat[i];
        if (flagInvert == 1)
        {
            src_bits = ~src_bits;
        }

        U8 preserve_bits = gLcdBuffer[curPage][posX1] & preserve_mask;
        U8 new_bits = (src_bits & src_mask) << page_off;
        gLcdBuffer[curPage][posX1] = preserve_bits | new_bits;

        if (span_pages)
        {
            const U8 page1 = curPage + 1;
            preserve_bits = gLcdBuffer[page1][posX1] & preserve_mask1;
            new_bits = src_bits >> page_rem;
            gLcdBuffer[page1][posX1] = preserve_bits | new_bits;
        }
    } // for
}

void SC5260_ClearArea(U8 posY, U8 posX, U8 width, U8 height, U8 fillData)
{

    if (height == 0 || width == 0)
    {
        return;
    }

    const uint32_t pages = (height + 7) / 8;

    for (uint32_t i = 0; i < pages; i++)
    {
        if (i < pages - 1)
        {
            SC5260_DisplaySmallArea(posY, posX, width, 8, NULL, TRUE, fillData);
        }
        else
        {
            U8 height1 = height % 8;
            if (0 == height1)
            {
                height1 = 8;
            }
            SC5260_DisplaySmallArea(posY, posX, width, height1, NULL, TRUE, fillData);
        }

        posY += 8;
    }
}

void SC5260_DisplayArea(U8 posY, U8 posX, U8 width, U8 height, const U8 *pdat, U8 flagInvert)
{
    if (height == 0 || width == 0)
    {
        return;
    }

    const uint32_t pages = (height + 7) / 8;

    for (uint32_t i = 0; i < pages; i++)
    {
        if (i != (pages - 1))
        {
            SC5260_DisplaySmallArea(posY, posX, width, 8, &pdat[i * width], 0, flagInvert);
        }
        else
        {
            U8 height1 = height % 8;
            if (0 == height1)
            {
                height1 = 8;
            }
            SC5260_DisplaySmallArea(posY, posX, width, height1, &pdat[i * width], 0, flagInvert);
        }

        posY += 8;
    }
}

// 全屏更新
extern void LCD_UpdateFullScreen(void)
{
    U8 i, j;

    for (i = 0; i < 8; i++)
    {
        SC5260_SetStartPosition(i, 4);

        LCD_CS_L;
        LCD_RS_H;
        SC5260_delay(1);
        for (j = 0; j < 128; j++)
        {
            if (g_radioInform.DisplayStyles)
            {
                SC5260_writeDat(~gLcdBuffer[i][j]);
            }
            else
            {
                SC5260_writeDat(gLcdBuffer[i][j]);
            }
        }
    }
    SC5260_delay(1);
    LCD_CS_H;
}

// 更新状态栏
extern void LCD_UpdateStateBar(void)
{
    U8 j;

    SC5260_SetStartPosition(0, 4);

    LCD_CS_L;
    LCD_RS_H;
    SC5260_delay(1);
    for (j = 0; j < 128; j++)
    {
        if (g_radioInform.DisplayStyles)
        {
            SC5260_writeDat(~gLcdBuffer[0][j]);
        }
        else
        {
            SC5260_writeDat(gLcdBuffer[0][j]);
        }
    }
    SC5260_delay(1);
    LCD_CS_H;
}

// 更新工作区域
extern void LCD_UpdateWorkAre(void)
{
    U8 i, j;

    for (i = 1; i < 8; i++)
    {
        SC5260_SetStartPosition(i, 4);

        LCD_CS_L;
        LCD_RS_H;
        SC5260_delay(1);
        for (j = 0; j < 128; j++)
        {
            if (g_radioInform.DisplayStyles)
            {
                SC5260_writeDat(~gLcdBuffer[i][j]);
            }
            else
            {
                SC5260_writeDat(gLcdBuffer[i][j]);
            }
        }
    }
    SC5260_delay(1);
    LCD_CS_H;
}

extern void LCD_ClearStateBar(void)
{
    memset(gLcdBuffer, 0x00, 128);
}

extern void LCD_ClearWorkArea(void)
{
    memset(&gLcdBuffer[1][0], 0x00, 128 * 7);
}

// 清除液晶缓存
extern void LCD_ClearFullBuf(void)
{
    memset(gLcdBuffer, 0x00, sizeof(gLcdBuffer));
}
