#include "includes.h"

extern void DisplayBatteryVol()
{
    UserADC_GetValOfBatt();
    uint8_t adc_val = UserADC_GetValOfBatt();
    uint32_t mv = get_battery_voltage(adc_val);

    do
    {
        const char *str = "Battery";
        uint32_t len = strlen(str);
        LCD_DisplayText(2 * 8, (128 - len * 8) / 2, (U8 *)str, FONTSIZE_16x16, LCD_DIS_NORMAL);
    } while (0);

    do
    {
        char buf[32];
        sprintf(buf, "%d.%02d V", //
                mv / 1000,        //
                (mv % 1000) / 10  //
        );

        uint32_t len = strlen(buf);
        LCD_DisplayText(4 * 8, (128 - len * 8) / 2, (U8 *)buf, FONTSIZE_16x16, LCD_DIS_NORMAL);
    } while (0);
}
