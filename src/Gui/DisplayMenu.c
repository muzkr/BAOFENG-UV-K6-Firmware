#include "includes.h"

#define EnS "abc"
#define EnL "ABC"
#define Num "123"
#define Pinyin " PY"

String disBuf[17];

const STR_MENU_ITEM MenuList[] =
    {
        {vo_Null, "CHL Name", "�ŵ�����"},
        {vo_Null, "RX Frequency", "����Ƶ��"},
        {vo_Null, "TX Frequency", "����Ƶ��"},
        {vo_CTCSS, "R-CTCSS", "����ģ������"},
        {vo_DCS, "R-DCS", "������������"},
        {vo_CTCSS, "T-CTCSS", "����ģ������"},
        {vo_DCS, "T-DCS", "������������"},
        {vo_Chlbandwidth, "BandWidth", "�ŵ�����"},
        {vo_power, "TXP", "���书��"},
        {vo_Null, "Silence Mode", "����ģʽ"},
        {vo_Null, "TX Forbid", "�����ֹ"},
        {vo_Freqdir, "Shift DIR", "Ƶ���"},
        {vo_Offsetfreq, "OFFSET", "Ƶ��Ƶ��"},
        {vo_Memorychl, "MEMCH", "�ŵ��洢"},
        {vo_Deletechl, "DELCH", "�ŵ�ɾ��"},
        {vo_Null, "VFO FreqRang", "VFOɨ�跶Χ"},
        {vo_Null, "Scan Mode", "ɨ��ģʽ"},
        {vo_Null, "DTMFST", "��������"},
        {vo_Null, "PTT ID", "DTMF����"},
        {vo_Null, "PTT-LT", "���븽���ӳ�"},
        {vo_Squelch, "Squelch", "����ȼ�"},
        {vo_savemode, "RX Save", "ʡ��ģʽ"},
        {vo_VOX, "VOX Switch", "���ؿ���"},
        {vo_Null, "VOX Level", "���صȼ�"},
        {vo_Null, "VOX Delay", "������ʱ"},
        {vo_Txovertime, "TX OVer Time", "���䳬ʱ"},
        {vo_Voicepromrt, "Language", "�˵�����"},
        {vo_Null, "VOICE", "������ʾ����"},
        {vo_Null, "Menu HangTime", "�˵�����ʱ��"},
        {vo_Beepprompt, "BEEP PROMPT", "����������"},
        {vo_Null, "Roger Beep", "ͨ��������ʾ��"},
        {vo_Null, "POWER ON TYPE", "��������ѡ��"},
        {vo_Null, "Power On Tone", "������ʾ��"},
        {vo_Null, "Power on MSG", "������Ϣ"},
        {vo_Null, "POWER ON PWD", "��������"},
        {vo_Dualstandby, "Dual Watch", "˫Ƶ�غ�"},
        {vo_Null, "MDF-A", "A����ʾģʽ"},
        {vo_Null, "MDF-B", "B����ʾģʽ"},
        {vo_Null, "RP-STE", "�м�β������"},
        {vo_Null, "RPT-RL", "�м�β����ʱ"},
        {vo_Null, "ALERT", "�м̼�����Ƶ"},
        {vo_Step, "Freq Step", "����Ƶ��"},
        {vo_Busylockout, "Busy Lockout", "��æ����"},
        {vo_Null, "Side Tone", "β������"},
        {vo_Null, "Alarm Mode", "����ģʽ"},
        {vo_Null, "PF1", "���1�̰�"},
        {vo_Null, "PF1 LONG PRESS", "���1����"},
        {vo_Null, "PF2", "���2�̰�"},
        {vo_Null, "ABR", "�Զ������"},
        {vo_Null, "Brightness", "LCD�Աȶ�"},
        {vo_Null, "LCD Reflex", "������Ļ"},
        {vo_Null, "AUTOLOCK", "�Զ�������"},
        {vo_Null, "Radio Interrupt", "�������"},
        {vo_initialization, "Reset", "�ָ���������"},
        {vo_Null, "STOP WATCH", "�������"},
        {vo_Null, "VERSION", "�汾��Ϣ"},
};

const STR_MENU_ITEM MenuFmList[] =
    {
        {vo_Null, "FM MEMCH", "FM�ŵ��洢"},
        {vo_Null, "FM BAND", "FM����Ƶ��"},
};

const String *VoxCnStr[] =
    {
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "10"};

const String *BatSaveCnStr[] =
    {
        "�ر�",
        "1:1",
        "1:2",
        "1:4",
};
const String *BatSaveEnStr[] =
    {
        "OFF",
        "1:1",
        "1:2",
        "1:4",
};

const String *OnOffCnStr[] =
    {
        "�ر�",
        "����"};
const String *OnOffEnStr[] =
    {
        "OFF",
        "ON"};

const String *OnSelCnStr[] =
    {
        "����"};
const String *OnSelEnStr[] =
    {
        "ON"};

const String *AlmodCnStr[] =
    {
        "�ֳ�����",
        "���ͱ�����",
        "���ͱ�����"};
const String *AlmodEnStr[] =
    {
        "ON SITE",
        "SEND SOUND",
        "SEND CODE"};

const String *ScanmodCnStr[] =
    {
        "ʱ��",
        "�ز�",
        "����"};
const String *ScanmodEnStr[] =
    {
        "TO",
        "CO",
        "SE"};

const String *PttIdSelCnStr[] =
    {
        "�ر�",
        "����PTT����",
        "�ɿ�PTT����",
        "ͬʱ����"};
const String *PttIdSelEnStr[] =
    {
        "OFF",
        "BOT",
        "EOT",
        "BOTH"};

const String *BandCnStr[] =
    {
        "խ��",
        "����",
};
const String *BandEnStr[] =
    {
        "WIDE",
        "NARROW",
};

const String *TxPowerCnStr[] =
    {
        "�߹���",
        "�͹���"};
const String *TxPowerEnStr[] =
    {
        "HIGH",
        "LOW"};

const String *VfoStepStr[] =
    {
        "2.5 K",
        "5.0 K",
        "6.25 K",
        "10.0 K",
        "12.5 K",
        "20.0 K",
        "25.0 K",
        "50.0 K",
};

const String *VfoDirCnStr[] =
    {
        "�ر�",
        "+",
        "-"};

const String *VfoDirEnStr[] =
    {
        "OFF",
        "+",
        "-"};

const String *ChDisCnStr[] =
    {
        "�ŵ�����",
        "�ŵ�Ƶ��",
        "�ŵ���",
        "����+Ƶ��"};

const String *ChDisEnStr[] =
    {
        "NAME",
        "FREQ",
        "CH",
        "NAME+FREQ"};

const String *LanStr[] =
    {
        "ENGLISH",
        "����"};

const String *DtmfSetSelCnStr[] =
    {
        "�ر�",
        "��������",
        "������",
        "����+������"};
const String *DtmfSetSelEnStr[] =
    {
        "OFF",
        "DT-ST",
        "ANI-ST",
        "DT+ANI"};

const String *DevResetCnStr[] =
    {
        "Ƶ��ģʽ",
        "ȫ��"};

const String *DevResetEnStr[] =
    {
        "VFO",
        "All"};

const String *ToneEnStr[] =
    {
        " 1000hz", // 0
        " 1450hz", // 1
        " 1750hz", // 2
        " 2100hz", // 3
};

const String *PwrOnCnStr[] =
    {
        "Ԥ��ͼ��",
        "Ԥ����Ϣ",
        "��ص�ѹ"};
const String *PwrOnEnStr[] =
    {
        "LOGO",
        "MESSAGE",
        "VOLTAGE"};

const String *SideKeyCnStr[] =
    {
        "��",
        "�ֵ�Ͳ����",
        "����ѡ��",
        "ɨ�迪��",
        "���ؿ���",
        "��������",
        "����������",
};
const String *SideKeyEnStr[] =
    {
        "None",
        "Torch On/Off",
        "Power Select ",
        "Scan On/Off",
        "VOX On/Off",
        "Alarm on/off",
        "Radio on/off",
};

const String *RxEndTailSelCnStr[] =
    {
        "�ر�",
        "MDC1200"};

const String *RxEndTailSelEnStr[] =
    {
        "OFF",
        "MDC1200"};

const String *TxEndToneCnStr[] =
    {
        "�ر�",
        "�����",
        "TONE1200"};

const String *TxEndToneEnStr[] =
    {
        "OFF",
        "BEEP",
        "TONE1200"};

const String *ReflexCnStr[] =
    {
        "����",
        "����"};

const String *ReflexEnStr[] =
    {
        "Normal",
        "Reflex"};

const String *PwrOnToneSelEnStr[] =
    {
        "None",
        "Tone",
        "Voice"};

const String *PwrOnToneSelCnStr[] =
    {
        "��",
        "����",
        "����"};

const String *SpMuteSelCnStr[] =
    {
        "����",
        "����+��ѡ����",
};

const String *SpMuteSelEnStr[] =
    {
        "CTDCS",
        "CTDCS+Signaling",
};

const String *DualSelCnStr[] =
    {
        "�ر�",
        "˫��˫��",
        "˫�ε���",
};

const String *DualSelEnStr[] =
    {
        "OFF",
        "Double Wait",
        "Signal Wait"};

const String *FMBandItemStr[] =
    {
        "76-108Mhz",
        "65-76Mhz",
};

const U8 PttIDDelay[] = {0, 1, 2, 4, 6, 8, 10};

extern void Menu_GetSubItemString(U8 menuIndex)
{
    if (g_menuInfo.menuType == 1)
    {
        switch (menuIndex)
        {
        case 0:
            if (CheckFmChActive(g_menuInfo.selectedItem))
            {
                sprintf(disBuf, "CH-%02d", g_menuInfo.selectedItem + 1);
            }
            else
            {
                sprintf(disBuf, "%02d", g_menuInfo.selectedItem + 1);
            }
            break;
        case 1:
            sprintf(disBuf, "%s", FMBandItemStr[g_menuInfo.selectedItem]);
        default:
            break;
        }
    }
    else
    {
        switch (menuIndex)
        {
        case S_CHNAME:
            sprintf(disBuf, "%s", g_inputbuf.buf);
            break;
        case S_RXFREQ:
        case S_TXFREQ:
        case S_RXCTS:
        case S_TXCTS:
        case S_OFFSE:
        case S_VFOSCAN:
            break;
        case S_RXDCS:
        case S_TXDCS:
            if (g_menuInfo.selectedItem == 211)
            {
                if (g_sysRunPara.decoderCode)
                {
                    sprintf(disBuf, "%06X", (g_sysRunPara.decoderCode & 0xFFFFFF));
                }
                else
                {
                    memset(disBuf, ' ', 16);
                }
            }
            else if (g_menuInfo.selectedItem == 0 || g_menuInfo.selectedItem > 210)
            {
                if (g_radioInform.language == LANG_CN)
                {
                    sprintf(disBuf, "%s", "�ر�");
                }
                else
                {
                    sprintf(disBuf, "%s", "OFF");
                }
            }
            else
            {
                if (g_menuInfo.selectedItem > 105)
                { // ����
                    sprintf(disBuf, "D%03oI", DCS_TAB[g_menuInfo.selectedItem - 106]);
                }
                else
                { // ����
                    sprintf(disBuf, "D%03oN", DCS_TAB[g_menuInfo.selectedItem - 1]);
                }
            }
            break;
        case S_WN:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", BandCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", BandEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_TXPR:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", TxPowerCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", TxPowerEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_SPMUTE:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", SpMuteSelCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", SpMuteSelEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_SFTD:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", VfoDirCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", VfoDirEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_MEMCH:
        case S_DELCH:
            if (CheckChannelActive(g_menuInfo.selectedItem, 0))
            {
                sprintf(disBuf, "CH-%03d", g_menuInfo.selectedItem + 1);
            }
            else
            {
                sprintf(disBuf, "%03d", g_menuInfo.selectedItem + 1);
            }
            break;
        case S_SCREV:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", ScanmodCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", ScanmodEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_DTST:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", DtmfSetSelCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", DtmfSetSelEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_PTTID:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", PttIdSelCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", PttIdSelEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_PTTLT:
            sprintf(disBuf, "%dMs", PttIDDelay[g_menuInfo.selectedItem] * 100);
            break;
        case S_SQL:
            sprintf(disBuf, "%d", g_menuInfo.selectedItem);
            break;
        case S_SAVE:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", BatSaveCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", BatSaveEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_VOXLV:
            sprintf(disBuf, "%s", VoxCnStr[g_menuInfo.selectedItem]);
            break;
        case S_VOXDLY:
        {
            U8 time;
            time = g_menuInfo.selectedItem + 5;
            sprintf(disBuf, "%d.%dsec", time / 10, time % 10);
        }
        break;
        case S_TOT:
            if (g_menuInfo.selectedItem == 0)
            {
                if (g_radioInform.language == LANG_CN)
                {
                    sprintf(disBuf, "%s", "�ر�");
                }
                else
                {
                    sprintf(disBuf, "%s", "OFF");
                }
            }
            else
            {
                sprintf(disBuf, "%dS", g_menuInfo.selectedItem * 15);
            }
            break;
        case S_LAN:
            sprintf(disBuf, "%s", LanStr[g_menuInfo.selectedItem]);
            break;
        case S_MENUEXIT:
            if (g_menuInfo.selectedItem == 10)
            {
                sprintf(disBuf, "60sec");
            }
            else
            {
                sprintf(disBuf, "%dsec", (g_menuInfo.selectedItem + 1) * 5);
            }
            break;

        case S_ROGE:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", TxEndToneCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", TxEndToneEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_PONTYPE:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", PwrOnCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", PwrOnEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_PONTONE:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", PwrOnToneSelCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", PwrOnToneSelEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_PONMSG:
        {
            U8 i = 0;
            memset(disBuf, 0x00, 16);
            for (i = 0; i < 16; i++)
            {
                if (powerOnMsg[i] == 0xFF || powerOnMsg[i] == 0x00)
                {
                    break;
                }
                disBuf[i] = powerOnMsg[i];
            }
        }
        break;
        case S_TDR:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", DualSelCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", DualSelEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_MDF1:
        case S_MDF2:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", ChDisCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", ChDisEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_RPSTE:
        case S_RPTRL:
            if (g_menuInfo.selectedItem == 0)
            {
                if (g_radioInform.language == LANG_CN)
                {
                    sprintf(disBuf, "%s", "�ر�");
                }
                else
                {
                    sprintf(disBuf, "%s", "OFF");
                }
            }
            else
            {
                sprintf(disBuf, "%dS", g_menuInfo.selectedItem);
            }
            break;
        case S_RTONE:
            sprintf(disBuf, "%s", ToneEnStr[g_menuInfo.selectedItem]);
            break;
        case S_STEP:
            sprintf(disBuf, "%s", VfoStepStr[g_menuInfo.selectedItem]);
            break;
        case S_ALMOD:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", AlmodCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", AlmodEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_SK1:
        case S_SKL1:
        case S_SK2:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", SideKeyCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", SideKeyEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_ABR:
            if (g_menuInfo.selectedItem == 0)
            {
                if (g_radioInform.language == LANG_CN)
                {
                    sprintf(disBuf, "%s", "�ر�");
                }
                else
                {
                    sprintf(disBuf, "%s", "OFF");
                }
            }
            else
            {
                if (g_menuInfo.selectedItem < 5)
                {
                    sprintf(disBuf, "%dsec", g_menuInfo.selectedItem * 5);
                }
                else if (g_menuInfo.selectedItem == 5)
                {
                    sprintf(disBuf, "30sec");
                }
                else
                {
                    sprintf(disBuf, "%dmin", (g_menuInfo.selectedItem - 5));
                }
            }
            break;
        case S_BRIGHT:
            sprintf(disBuf, "%d", g_menuInfo.selectedItem + 1);
            break;
        case S_REFLEX:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", ReflexCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", ReflexEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_AUTOLK:
            if (g_menuInfo.selectedItem == 0)
            {
                if (g_radioInform.language == LANG_CN)
                {
                    sprintf(disBuf, "%s", "�ر�");
                }
                else
                {
                    sprintf(disBuf, "%s", "OFF");
                }
            }
            else
            {
                sprintf(disBuf, "%dS", g_menuInfo.selectedItem * 5);
            }
            break;
        case S_RESET:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", DevResetCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", DevResetEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_TXFORBID:
        case S_VOX:
        case S_VOIC:
        case S_BEEP:
        case S_PWR:
        case S_BUSYLOCK:
        case S_TAIL:
        case S_FMINT:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", OnOffCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", OnOffEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_WATCH:
            if (g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf, "%s", OnSelCnStr[g_menuInfo.selectedItem]);
            }
            else
            {
                sprintf(disBuf, "%s", OnSelEnStr[g_menuInfo.selectedItem]);
            }
            break;
        case S_INFO:
            if (g_menuInfo.selectedItem == 0)
            {
                if (g_radioInform.language == LANG_CN)
                {
                    sprintf(disBuf, "�����汾:%s", "V0.01");
                }
                else
                {
                    sprintf(disBuf, "Firmware:%s", "V01  ");
                }
            }
            else
            {
                if (g_radioInform.language == LANG_CN)
                {
                    sprintf(disBuf, "Ӳ���汾:%s", "V0.01");
                }
                else
                {
                    sprintf(disBuf, "Hardware:%s", "V01  ");
                }
            }
            break;
        default:
            break;
        }
    }
}

extern void Menu_DisplayFreqError(void)
{
    String disBuf[17];

    if (g_radioInform.language == LANG_CN)
    {
        TranStrToMiddle(disBuf, (String *)"Ƶ�ʳ�����Χ!", 16);
    }
    else
    {
        TranStrToMiddle(disBuf, (String *)"out of range!", 16);
    }
    LCD_DisplayText(47, 0, (U8 *)disBuf, FONTSIZE_16x16, 0);
    LCD_UpdateWorkAre();

    // ��ʱ1S��ʾ����ɹ�����
    DelaySysMs(500);
}

void DisplayInputOffect(U32 selItem)
{
    U8 i, j;
    String buf[10] = {0};

    selItem = g_menuInfo.inputVal / 10;
    if (g_menuInfo.isSubMenu == 0)
    {
        sprintf(disBuf, "%d.%04d", selItem / 10000, selItem % 10000);
    }
    else
    {
        if (g_menuInfo.inputVal)
        {
            sprintf(disBuf, "%d.%04d", selItem / 10000, selItem % 10000);
        }
        else
        {
            memset(buf, '-', 7);
            buf[2] = '.';

            j = 0;
            for (i = 0; i < g_inputbuf.len; i++)
            {
                if (i == 2)
                {
                    j++;
                }
                buf[j++] = g_inputbuf.buf[i];
            }
            buf[7] = 0;
            strncpy(disBuf, buf, 16);
        }
    }
}
void DisplayInputChFreq(U32 selItem)
{
    U8 i, j;
    String buf[10] = {0};

    selItem = g_menuInfo.inputVal;
    if (g_menuInfo.isSubMenu == 0)
    {
        sprintf(disBuf, "%d.%05d", selItem / 100000, selItem % 100000);
    }
    else
    {
        if (g_menuInfo.inputVal)
        {
            sprintf(disBuf, "%d.%05d", selItem / 100000, selItem % 100000);
        }
        else
        {
            memset(buf, '-', 7);
            buf[3] = '.';

            j = 0;
            for (i = 0; i < g_inputbuf.len; i++)
            {
                if (i == 3)
                {
                    j++;
                }
                buf[j++] = g_inputbuf.buf[i];
            }
            buf[7] = 0;
            strncpy(disBuf, buf, 16);
        }
    }
}
void DisplayInputVfoScan(U32 selItem)
{
    U8 i, j;
    String buf[10] = {0};

    selItem = g_menuInfo.inputVal;
    if (g_menuInfo.isSubMenu == 0)
    {
        sprintf(disBuf, "%03d-%03d", selItem / 1000, selItem % 1000);
    }
    else
    {
        if (g_menuInfo.inputVal)
        {
            sprintf(disBuf, "%03d-%03d", selItem / 1000, selItem % 1000);
        }
        else
        {
            memset(buf, '-', 7);

            j = 0;
            for (i = 0; i < g_inputbuf.len; i++)
            {
                if (i == 3)
                {
                    j++;
                }
                buf[j++] = g_inputbuf.buf[i];
            }
            buf[7] = 0;
            strncpy(disBuf, buf, 16);
        }
    }
}

static U8 CheckCtcssInList(U16 ctcssDat)
{
    U8 i;

    if (ctcssDat == 0)
    {
        return 0;
    }

    // �ж�����Ƶ�Ƿ���������
    for (i = 0; i < 51; i++)
    {
        if (CTCS_TAB[i] == ctcssDat)
        {
            return (i);
        }
    }

    return 0xFF;
}

void GetCtcssDisBuf(U16 Index)
{
    if (Index == 0)
    {
        if (g_radioInform.language == LANG_CN)
        {
            sprintf(disBuf, "�ر�");
        }
        else
        {
            sprintf(disBuf, "OFF");
        }
    }
    else if (Index == 0xFF)
    {
        sprintf(disBuf, "%d.%dHz", g_menuInfo.inputVal / 10, g_menuInfo.inputVal % 10);
    }
    else
    {
        sprintf(disBuf, "%d.%dHz", CTCS_TAB[Index] / 10, CTCS_TAB[Index] % 10);
    }
}

static void ShowCtcssList(void)
{
    U8 selecteId;

    if (g_menuInfo.isSubMenu == 0)
    { // �ڵ�һ��ѡ��˵�ѡ��

        if (g_menuInfo.inputVal == 0)
        {
            selecteId = 0;
        }
        else
        {
            selecteId = CheckCtcssInList(g_menuInfo.inputVal);
        }
        GetCtcssDisBuf(selecteId);
    }
    else
    {
        selecteId = CheckCtcssInList(g_menuInfo.inputVal);
        // selecteId = g_menuInfo.selectedItem;
        GetCtcssDisBuf(selecteId);
    }
}

extern void UpdateMenuDisplay(void)
{
    String lcdDisBuf[17] = {0}, headbuf[17] = {0};

    if (g_menuInfo.menuType == 1)
    {
        if (g_radioInform.language == LANG_CN)
        {
            strcpy(headbuf, MenuFmList[g_menuInfo.menuIndex].nameCn);
        }
        else
        {
            strcpy(headbuf, MenuFmList[g_menuInfo.menuIndex].nameEn);
        }
    }
    else
    {
        if (g_radioInform.language == LANG_CN)
        {
            strcpy(headbuf, MenuList[g_menuInfo.menuIndex].nameCn);
        }
        else
        {
            strcpy(headbuf, MenuList[g_menuInfo.menuIndex].nameEn);
        }
    }
    TranStrToMiddle(lcdDisBuf, headbuf, 12);
    LCD_DisplayText(20, 16, (U8 *)lcdDisBuf, FONTSIZE_16x16, LCD_DIS_NORMAL);

    sprintf(lcdDisBuf, "%02d", g_menuInfo.menuIndex);
    LCD_DisplayNumber(18, 2, (U8 *)lcdDisBuf, 0);

    TranStrToMiddle(lcdDisBuf, disBuf, 16);
    LCD_DisplayText(47, 0, (U8 *)lcdDisBuf, FONTSIZE_16x16, 0);

    if (g_menuInfo.isSubMenu)
    {
        LCD_DisplayPicture(50, 0, ICON_MENU_SEL_SIZEX, ICON_MENU_SEL_SIZEY, iconMenuSel, LCD_DIS_NORMAL);
    }

    if (g_menuInfo.inputMode == MENU_ONE_CTCSS || g_menuInfo.inputMode == MENU_ONE_DECODE)
    {
        if (g_menuInfo.inputMode == MENU_ONE_DECODE)
        {
            sprintf(lcdDisBuf, "%03d", g_menuInfo.selectedItem);
        }
        else
        {
            sprintf(lcdDisBuf, " %02d", g_menuInfo.selectedItem);
        }
        LCD_DisplayNumber(47, 109, (U8 *)lcdDisBuf, 0);
    }

    DisplayStateBar();
    LCD_UpdateWorkAre();
}

extern void DisplayInputType(void)
{
    String *str;
    String buf[6];

    if (inputTypeBack != g_inputbuf.inputType)
    {
        inputTypeBack = g_inputbuf.inputType;
        switch (g_inputbuf.inputType)
        {
        case IN_EN_L:
            str = EnS;
            break;
        case IN_EN_U:
            str = EnL;
            break;
        case IN_PINYIN:
            str = Pinyin;
            break;
        case IN_NUMBER:
        default:
            str = Num;
            break;
        }
        sprintf(buf, "%s", str);
        SC5260_ClearArea(10, 107, 20, 9, 1);
        // ��ʾ��������
        LCD_DisplayNumber(11, 108, (U8 *)buf, 1);
        LCD_UpdateWorkAre();
    }
}

const U8 InputPosY[] = {13, 32};

void DisplaySelectHz(void)
{
    U8 i;
    String displayBuf[19];

    for (i = 0; i < 2; i++)
    {
        if (lcdDispBuf[i][0] == 0x08)
        { // ��һת���ַ�ʱ����ʾ���ᵼ���ܳ���+1
            sprintf(displayBuf, "%-*.*s", 17, 17, lcdDispBuf[i]);
            displayBuf[16 + 1] = 0;
        }
        else
        {
            sprintf(displayBuf, "%-*.*s", 16, 16, lcdDispBuf[i]);
            displayBuf[16] = 0;
        }
        SC5260_ClearArea(30, 2, 124, 1, 1);
        LCD_DisplayText(InputPosY[i], 4, (U8 *)displayBuf, FONTSIZE_16x16, 0);
    }
}

extern void MenuShowInputChar(void)
{
    DrowInputWindow();

    DisplayInputType();

    // ��ʾ������ַ���
    sprintf((String *)disBuf, "%*.*s", 16, 16, g_inputbuf.buf);
    // ��ʾ���������½�
    LCD_DisplayText(47, 0, (U8 *)disBuf, FONTSIZE_16x16, 0);
    uartSendChar(disBuf[0]);

    /*���ƺ��������*/
    if (pyBuf.pos != 0)
    {
        DisplaySelectHz();
    }

    inputTypeBack = 0xFF;
    DisplayInputType();
}

extern void Menu_Display(void)
{
    U8 i, posx;

    DisplayBattaryFlag(0);

    if (!(g_menuInfo.inputMode == MENU_ONE_CHAR && g_menuInfo.isSubMenu))
    {
        // �����м�����
        SC5260_ClearArea(41, 5, 121, 2, 1);
        posx = 5;
        for (i = 0; i < 13; i++)
        {
            SC5260_ClearArea(40, posx, 1, 1, 1);
            posx += 10;
        }
    }
    switch (g_menuInfo.inputMode)
    {
    case MENU_ONE_CHAR:
        if (g_menuInfo.isSubMenu)
        { // ��ʾ�ŵ����Ʊ༭����
            LCD_ClearWorkArea();
            MenuShowInputChar();
            return;
        }
        else
        {
            Menu_GetSubItemString(g_menuInfo.menuIndex);
        }
        break;
    case MENU_ONE_FREQ: // Ƶ������
        if (g_menuInfo.isSubMenu == 0)
        {
            g_menuInfo.inputVal = g_menuInfo.selectedItem;
        }
        DisplayInputOffect(g_menuInfo.inputVal);

        break;
    case MENU_CH_FREQ: // Ƶ������
        if (g_menuInfo.isSubMenu == 0)
        {
            g_menuInfo.inputVal = g_menuInfo.selectedItem;
        }
        DisplayInputChFreq(g_menuInfo.inputVal);

        break;
    case MENU_ONE_VFOSCAN: // Ƶ��ɨ�跶Χ
        if (g_menuInfo.isSubMenu == 0)
        {
            g_menuInfo.inputVal = g_menuInfo.selectedItem;
        }
        DisplayInputVfoScan(g_menuInfo.inputVal);
        break;

    case MENU_ONE_CTCSS: // ģ������Ƶѡ�������
        ShowCtcssList();
        break;
    case MENU_ONE_DIGIT:
        sprintf(disBuf, "%d", g_menuInfo.selectedItem);
        break;
    case MENU_ONE_SELECT:
    default:
        Menu_GetSubItemString(g_menuInfo.menuIndex);
        break;
    }
    UpdateMenuDisplay();
}
