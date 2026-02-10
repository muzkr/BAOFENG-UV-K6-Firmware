#include "includes.h"

STR_KEYSCAN g_keyScan;
static U8 PressedKey = KEYID_NONE;
static U8 KeyPressCount;
static U8 SubmitKey = 0;

#define KeyReadDelay() DelayUs(5)

// Choose "Lx" in this order: Side keys, L0, L1, L2, L3, L4
static const U16 keyscanTab1[] = {0x0E11, 0x0E10, 0x0E01, 0x0A11, 0x0611, 0x0C11}; // 扫描列表_输出高电平
static const U16 keyscanTab2[] = {0x0000, 0x0001, 0x0010, 0x0400, 0x0800, 0x0200}; // 扫描列表_输出低电平

// 矩阵键值
static const KeyID_Enum KEYBOARD_TABLE[][4] = {
    // K0, K1, K2, K3
    {KEYID_NONE, KEYID_SIDEKEY1, KEYID_SIDEKEY2, KEYID_NONE}, // Side keys
    {KEYID_7, KEYID_8, KEYID_9, KEYID_NUM},                   // L0
    {KEYID_4, KEYID_5, KEYID_6, KEYID_0},                     // L1
    {KEYID_1, KEYID_2, KEYID_3, KEYID_STAR},                  // L2
    {KEYID_MENU, KEYID_UP, KEYID_DOWN, KEYID_EXIT},           // L3
    {KEYID_VM, KEYID_AB, KEYID_BAND, KEYID_NONE},             // L4
};

/**
 *  Read "Kx" pins input
 */
static U16 KEY_ReadGpioInput()
{
    uint16_t KK = 0;

    // K0, K1
    uint16_t n1 = GPIO_ReadInputData(GPIOC) & (GPIO_Pin_14 | GPIO_Pin_13);
    KK |= (n1 >> 13);

    // K2
    n1 = GPIO_ReadInputData(GPIOB) & GPIO_Pin_14;
    KK |= (n1 >> (14 - 2));

    // K3
    n1 = GPIO_ReadInputData(GPIOF) & GPIO_Pin_6;
    KK |= (n1 >> (6 - 3));

    return KK;
}

KeyID_Enum GetKeyCode(void)
{
    for (uint32_t L = 0; L < 6; L++)
    {
        GPIO_SetBits(GPIOB, keyscanTab1[L]);
        GPIO_ResetBits(GPIOB, keyscanTab2[L]);

        KeyReadDelay();

        const uint16_t KK = KEY_ReadGpioInput();

        for (uint32_t K = 0; K < 4; K++)
        {
            if (0 == (1 & (KK >> K)))
            {
                return KEYBOARD_TABLE[L][K];
            }
        }
    }

    return KEYID_NONE;
}

extern void KeyScanReset(void)
{
    PressedKey = KEYID_NONE;
    g_keyScan.keyEvent = KEYID_NONE;
    g_keyScan.keyPara = KEY_CLICKED;
}

extern Boolean KEY_GetKeyEvent(void)
{
    static KeyID_Enum preKey = KEYID_NONE;
    static KeyID_Enum key = KEYID_NONE;

    preKey = key;
    key = GetKeyCode();

    if (preKey != key)
    {
        return FALSE;
    }

    if (key == KEYID_NONE)
    {
        if (PressedKey == KEYID_NONE)
        {
            KeyPressCount = 0;
            return FALSE;
        }

        if (SubmitKey == 0 || g_rfState == RF_TX)
        {
            KeyPressCount = 0;
            g_keyScan.keyEvent = PressedKey;
            g_keyScan.keyPara = KEYSTATE_RELEASE;
            g_keyScan.longPress = KEY_CLICKED;
            PressedKey = KEYID_NONE;
            return TRUE;
        }
        PressedKey = KEYID_NONE;
        SubmitKey = 0;
        return FALSE;
    }

    if (PressedKey == KEYID_NONE)
    { // 判断是否支持长按键
        PressedKey = key;
        KeyPressCount = 1;

        if (g_rfState == RF_TX)
        {
            g_keyScan.keyPara = KEYSTATE_CLICKED;
            g_keyScan.keyEvent = PressedKey;
            g_keyScan.longPress = KEY_CLICKED;
            return TRUE;
        }
    }

    if (PressedKey == key)
    {
        KeyPressCount++;

        if (KeyPressCount == 100)
        { // 支持长按键
            if (PressedKey == KEYID_UP || PressedKey == KEYID_DOWN || PressedKey == KEYID_VM || PressedKey == KEYID_BAND || PressedKey == KEYID_STAR || PressedKey == KEYID_NUM || PressedKey == KEYID_0 || PressedKey == KEYID_8 || PressedKey == KEYID_EXIT || PressedKey == KEYID_SIDEKEY1 || PressedKey == KEYID_SIDEKEY2)
            {
                g_keyScan.keyEvent = PressedKey;
                g_keyScan.keyPara = KEYSTATE_LONG;
                g_keyScan.longPress = KEY_LONG;
                SubmitKey = 1;
                return TRUE;
            }
        }

        if (KeyPressCount >= 150)
        {
            KeyPressCount = 140;

            if (PressedKey == KEYID_UP || PressedKey == KEYID_DOWN)
            {
                g_keyScan.keyEvent = PressedKey;
                g_keyScan.keyPara = KEYSTATE_NONE;
                g_keyScan.longPress = KEY_CONTINUE;

                SubmitKey = 1;
                return TRUE;
            }
        }
    }
    else
    { // 新按键按下
        if (SubmitKey == 0 || g_rfState == RF_TX)
        {
            g_keyScan.keyEvent = PressedKey;
            g_keyScan.longPress = KEY_CLICKED;
            g_keyScan.keyPara = KEYSTATE_RELEASE;
            KeyPressCount = 0;
            PressedKey = KEYID_NONE;
            return TRUE;
        }
        KeyPressCount = 0;
        PressedKey = KEYID_NONE;
    }
    return FALSE;
}

extern void KEY_ScanTask(void)
{
    if (KEY_GetKeyEvent() == FALSE)
    {
        return;
    }

    g_sysRunPara.rfTxFlag.voxDetDly = 4;

    if (g_radioInform.keyLock && (g_keyScan.keyEvent != KEYID_SIDEKEY1 && g_keyScan.keyEvent != KEYID_SIDEKEY2 && !(g_keyScan.keyEvent == KEYID_STAR && g_keyScan.longPress == KEY_LONG)))
    {
        g_keyScan.keyEvent = KEYID_NONE;
        g_keyScan.longPress = 0;
        BeepOut(BEEP_NULL);
    }

    LCD_BackLightSetOn();
    ResetTimeKeyLockAndPowerSave();
}

extern U8 Key_GetRealEvent(void)
{
    U8 keyEvent;

    if (g_keyScan.longPress == KEY_LONG)
    {
        switch (g_keyScan.keyEvent)
        {
        case KEYID_SIDEKEY1:
            keyEvent = KEYID_SIDEKEYL1;
            break;
        case KEYID_SIDEKEY2:
            keyEvent = KEYID_SIDEKEYL2;
            break;
        case KEYID_VM:
            keyEvent = KEYID_CHDIS;
            break;
        case KEYID_AB:
            keyEvent = KEYID_DUAL_SW;
            break;
        case KEYID_BAND:
        case KEYID_EXIT:
            keyEvent = KEYID_SEARCH;
            break;
        case KEYID_STAR:
            keyEvent = KEYID_LOCK;
            break;
        case KEYID_NUM:
            keyEvent = KEYID_SCAN;
            break;
        case KEYID_0:
            keyEvent = KEYID_WX;
            break;
        case KEYID_8:
            keyEvent = KEYID_PWRSW;
            break;
        default:
            keyEvent = KEYID_NONE;
            break;
        }
    }
    else if (g_keyScan.longPress == KEY_CONTINUE)
    {
        if (g_keyScan.keyEvent == KEYID_DOWN)
        {
            keyEvent = KEYID_DOWN_CONTINUE;
        }
        else
        {
            keyEvent = KEYID_UP_CONTINUE;
        }
    }
    else
    {
        keyEvent = g_keyScan.keyEvent;
    }

    g_keyScan.longPress = KEY_CLICKED;
    g_keyScan.keyEvent = KEYID_NONE;

    return keyEvent;
}
