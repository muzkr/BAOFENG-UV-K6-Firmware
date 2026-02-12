#include "includes.h"
#include "printf.h"

extern void UI_DisplayPowerOn(void)
{

    SC5620_SetContrastRatio(g_radioInform.brightness);

    // printf("power on type: %d\n", g_radioInform.OpFlag1.Bit.b0);

    if (g_radioInform.OpFlag1.Bit.b0 == 1)
    {
        uint32_t len = 0;
        for (; len < 16; len++)
        {
            if (powerOnMsg[len] == 0xFF || powerOnMsg[len] == 0x00)
            {
                break;
            }
        }

        // printf("power on message len: %d\n", len);

        LCD_DisplayText(3 * 8, (128 - len * 8) / 2, powerOnMsg, FONTSIZE_16, LCD_DIS_NORMAL);
    }
    else if (g_radioInform.OpFlag1.Bit.b0 == 2)
    {
        DisplayBatteryVol();
    }
    else
    {
        U8 picDisBuf[1024];
        SpiFlash_ReadBytes(FLASH_PON_MSG_ADDR, picDisBuf, 1024);
        SC5260_DisplayArea(0, 0, 128, 64, picDisBuf, LCD_DIS_NORMAL);
    }

    LCD_UpdateFullScreen();

    LcdBackLightSwitch(LED_ON);

    // Let caller do delay
    // DelayMs(500);
}
