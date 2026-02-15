#include "includes.h"

#define OFF_BYTE(n) ((uint8_t)(0xff << (n)))

U8 g_VfoBuf[66] = {0x00};
U8 g_powerOnMsg[17] = {0};
STR_RF_MODEL g_rfModel;
STR_BAND g_bandRang;

/*********************************************************************
 * 函 数 名: Flash_ModifyChannelData
 * 功能描述: 保存信道信息，包括信息数据和信道名称
 * 输入参数：channelNum:信道号  chData:数据指针  chName:信道名称数据指针
 * 输出参数:
 * 返　　回:
 * 说    明：
 ***********************************************************************/
void Flash_ModifyChannelData(U16 channelNum, const U8 *chData, const U8 *chName)
{
    // 计算存储信道逻辑地址
    U32 indexSector = channelNum / 128; // 4k / 32byte = 128
    // 换算信道位置
    U32 indexChannelNum = channelNum % 128;

    const U32 addr = CHAN_ADDR + (indexSector * 0x1000);

    U8 buf[4 * 1024]; // 4K

    // 读出数据/擦除FLASH
    SpiFlash_ReadBytes(addr, buf, 4 * 1024);
    SpiFlash_EraseSector(addr);

    // 修改信道数据，不改变信道名称
    memcpy(buf + (indexChannelNum * CHAN_SIZE), chData, sizeof(STR_CHANNEL));
    // 保存信道名称
    memcpy(buf + (indexChannelNum * CHAN_SIZE + NAME_ADDR_SHIFT), chName, NAME_SIZE);

    SpiFlash_WriteBytes(addr, buf, 4 * 1024);
}

/*********************************************************************
 * 功能描述: 保存信道信息，包括信息数据和信道名称
 * 输入参数：channelNum:信道号  chData:信道信息数据指针 chName:信道名称数据指针
 * 输出参数:
 * 返　　回:
 * 说    明：数据有改变时才会进行数据存储操作
 ***********************************************************************/
void Flash_SaveChannelData(U16 channelNum, const U8 *chData, const U8 *chName)
{
    U32 addr = CHAN_ADDR + channelNum * CHAN_SIZE;
    U8 channelBuf[CHAN_SIZE];
    SpiFlash_ReadBytes(addr, channelBuf, CHAN_SIZE);

    // 判断数据是否有改变
    if (memcmp(channelBuf, chData, sizeof(STR_CHANNEL)) != 0        //
        || memcmp(chName, &chData[NAME_ADDR_SHIFT], NAME_SIZE) != 0 //
    )
    {
        Flash_ModifyChannelData(channelNum, chData, chName);
    }
}

/*********************************************************************
 * 功能描述: 删除指定信道数据
 * 输入参数：channelNum:信道号
 * 输出参数:
 * 返　　回:
 * 说    明：只删除前8个字节频率数据，将数据清0
 ***********************************************************************/
void Flash_DeleteChannelData(U16 channelNum)
{
    U32 addr = CHAN_ADDR + channelNum * CHAN_SIZE;
    U8 resetBuf[8];
    memset(resetBuf, 0x00, 8);
    SpiFlash_WriteBytes(addr, resetBuf, 8);
}

/*********************************************************************
 * 函 数 名: Flash_GetLogicAddrShift
 * 功能描述: 获取地址偏移位置
 * 输入参数：addr:存储块地址   useByte:需要使用到的byte (bit/8)
 * 输出参数:
 * 返　　回:
 * 说    明：
 ***********************************************************************/
static U16 Flash_GetLogicAddrShift(U32 addr, U8 useByte)
{
    U8 addrMap[32];

    SpiFlash_ReadBytes(addr, addrMap, useByte);

    for (uint32_t i = 0; i < useByte; i++)
    {
        if (0 == addrMap[i])
        {
            continue;
        }

        for (uint32_t j = 0; j < 8; j++)
        {
            if (addrMap[i] & (1 << j))
            {
                return i * 8 + j;
            }
        }
    }

    return 0xFF;
}

// 保存对讲机 对讲机功能信息/收音机信道信息
void Flash_SaveRadioInfoData(void)
{
    // 对讲机功能信息1+对讲机功能信息2+收音机功能信息 = 94Byte + 2Byte(CheckSum)
    //  (84 - 16Byte) / 96 = 42  衰减次数: 42

    U8 buf[96];
    memset(buf, 0, sizeof(buf));
    memcpy(buf, (U8 *)&g_radioInform.sqlLevel, sizeof(STR_RADIOINFORM));
    // 用于存储开机显示信息
    memcpy(buf + 64, g_powerOnMsg, 16);

    // 计算CRC校验
    U16 checkSum = CRC_ValidationCalc(buf, 94);
    memcpy(buf + 94, &checkSum, 2);

    // 读取存储位置信息
    U16 addrOffset = Flash_GetLogicAddrShift(RADIO_INFO_ADDR, 6);
    if (addrOffset < 41)
    {
        U32 logicAddr = RADIO_INFO_ADDR + 16 + addrOffset * 96;

        // 读取当前校验和比较
        U16 CheckSumRd;
        SpiFlash_ReadBytes(logicAddr + 94, (U8 *)&CheckSumRd, 2);

        if (CheckSumRd == checkSum)
        {
            U8 cmpBuf[80];
            SpiFlash_ReadBytes(logicAddr, cmpBuf, 80);
            if (memcmp(cmpBuf, buf, 80) == 0)
            { // 数据相同直接返回
                return;
            }
        }

        // 数据有改变才保存
        logicAddr += 96;
        SpiFlash_WriteBytes(logicAddr, buf, 96);

        // 标记地址工作块
        U8 dataMap = OFF_BYTE((addrOffset + 1) % 8);
        SpiFlash_WriteBytes((RADIO_INFO_ADDR + (addrOffset / 8)), &dataMap, 1);
    }
    else
    { // 擦除
        SpiFlash_EraseSector(RADIO_INFO_ADDR);
        SpiFlash_WriteBytes((RADIO_INFO_ADDR + 16), buf, 96);
    }
}

// 读取对讲机 频率模式数据/对讲机功能信息/收音机信息
void Flash_ReadRadioInfoData(void)
{
    // 对讲机功能信息1+对讲机功能信息2+收音机功能信息 = 94Byte + 2Byte(CheckSum)
    //  (4K - 16Byte) / 96 = 42  衰减次数: 42

    // 读取存储位置信息
    const uint16_t addrOffset = Flash_GetLogicAddrShift(RADIO_INFO_ADDR, 6);
    if (addrOffset < 42)
    {
        uint32_t logicAddr = RADIO_INFO_ADDR + 16 + addrOffset * 96;
        U8 buf[96];
        SpiFlash_ReadBytes(logicAddr, buf, 96);

        U16 checkSum;
        memcpy(&checkSum, &buf[94], 2);
        if (checkSum == CRC_ValidationCalc(buf, 94))
        { // 校验通过
            memcpy(&g_radioInform, buf, sizeof(STR_RADIOINFORM));
            memcpy(g_powerOnMsg, buf + 64, 16);
            return;
        }
    }

    // 数据错误，校验未通过/从备份区提取数据
    SpiFlash_EraseSector(RADIO_INFO_ADDR);
    ResetRadioFunData();
}

// 保存对讲机频率模式数据,使用一块flash 4K,前16byte用于指示工作块
// A段频率模式+B段频率模式 = 64 + 2Byte(CheckSum)
// (4K - 16Byte) / 66 = 61  衰减次数: 61
//  前16字节用于衰减算法
void Flash_SaveVfoData(U8 workAB)
{
    if (workAB == 0 || workAB == 1)
    {
        if (memcmp(g_VfoBuf + 32 * workAB, &g_ChannelVfoInfo.vfoInfo[workAB], 32) == 0)
        { // 数据未改变不保存
            return;
        }
        memcpy(g_VfoBuf + 32 * workAB, &g_ChannelVfoInfo.vfoInfo[workAB], 32);
    }
    else if (workAB == 0XFF)
    { // 写频用
    }
    else
    { // 复位专用
        memcpy(g_VfoBuf, &g_ChannelVfoInfo.vfoInfo[0], 32);
        memcpy(g_VfoBuf + 32, &g_ChannelVfoInfo.vfoInfo[1], 32);
    }

    U16 checkSum = CRC_ValidationCalc(g_VfoBuf, 64);
    memcpy(g_VfoBuf + 64, &checkSum, 2);

    const U16 addrOffset = Flash_GetLogicAddrShift(VFO_INFO_ADDR, 8);
    if (addrOffset < 60)
    { // 最后块地址时就需要擦除
        U32 logicAddr = VFO_INFO_ADDR + 16 + (addrOffset + 1) * 66;
        SpiFlash_WriteBytes(logicAddr, g_VfoBuf, 66);
        // 标记地址工作块
        U8 dataMap = OFF_BYTE((addrOffset + 1) % 8);
        SpiFlash_WriteBytes((VFO_INFO_ADDR + (addrOffset / 8)), &dataMap, 1);
    }
    else
    { // 整块擦除后写入
        SpiFlash_EraseSector(VFO_INFO_ADDR);
        SpiFlash_WriteBytes(VFO_INFO_ADDR + 16, g_VfoBuf, 66);
    }
}

// 读取对讲机频率模式数据
void Flash_ReadVfoData(U8 workAB)
{
    U16 addrOffset = Flash_GetLogicAddrShift(VFO_INFO_ADDR, 8);
    if (addrOffset < 61)
    {
        U32 logicAddr = VFO_INFO_ADDR + 16 + (addrOffset * 66);
        SpiFlash_ReadBytes(logicAddr, g_VfoBuf, 66);

        U16 checkSum;
        memcpy(&checkSum, &g_VfoBuf[64], 2);
        if (checkSum == CRC_ValidationCalc(g_VfoBuf, 64))
        { // 数据校验正确
            memcpy(&g_ChannelVfoInfo.vfoInfo[workAB], g_VfoBuf + 32 * workAB, 32);
            return;
        }
    }

    // 数据错误，从初始化数据恢复
    SpiFlash_EraseSector(VFO_INFO_ADDR);
    ResetVfoModeData();
}

// 保存对讲机信道号,天气预报信道号,收音机当前频率
// 预留14Byte + 2Byte(CheckSum)
// (4K - 32Byte) / 16 = 254  衰减次数: 254
//  前32字节用于衰减算法
void Flash_SaveSystemRunData(void)
{
    U8 buf[16] = {0x00};

    const U16 addrOffset = Flash_GetLogicAddrShift(SYSTEM_RUN_ADDR, 32);

    // 提取数据
    memcpy(buf, g_ChannelVfoInfo.channelNum, 4);

    // 计算CRC校验
    U16 checkSum = CRC_ValidationCalc(buf, 14);
    memcpy(buf + 14, &checkSum, 2);

    if (addrOffset < 253)
    {
        U32 logicAddr = SYSTEM_RUN_ADDR + 32 + (addrOffset + 1) * 16;
        SpiFlash_WriteBytes(logicAddr, buf, 16);
        // 标记地址工作块
        U8 dataMap = OFF_BYTE((addrOffset + 1) % 8);
        SpiFlash_WriteBytes(SYSTEM_RUN_ADDR + addrOffset / 8, &dataMap, 1);
    }
    else
    { // 整块擦除后写入
        SpiFlash_EraseSector(SYSTEM_RUN_ADDR);
        SpiFlash_WriteBytes(SYSTEM_RUN_ADDR + 32, buf, 16);
    }
}

// 读取对讲机信道号,天气预报信道号,收音机当前频率
// 预留14Byte + 2Byte(CheckSum)
// (4K - 32Byte) / 16 = 254  衰减次数: 254
// 前32字节用于衰减算法
void Flash_ReadSystemRunData(void)
{
    uint16_t addrOffset = Flash_GetLogicAddrShift(SYSTEM_RUN_ADDR, 32);
    if (addrOffset < 254)
    {
        uint32_t logicAddr = SYSTEM_RUN_ADDR + 32 + (addrOffset * 16);
        U8 buf[16];
        SpiFlash_ReadBytes(logicAddr, buf, 16);

        U16 checkSum;
        memcpy(&checkSum, &buf[14], 2);
        if (checkSum == CRC_ValidationCalc(buf, 14))
        { // 数据校验正确
            memcpy(g_ChannelVfoInfo.channelNum, buf, 4);
            return;
        }
    }

    // 数据错误，从初始化数据恢复
    g_ChannelVfoInfo.channelNum[0] = 0;
    g_ChannelVfoInfo.channelNum[1] = 0;
}

// 保存DTMF本机ID
extern void Flash_SaveDtmfInfo(void)
{
    U8 buf[288] = {0xFF};

    SpiFlash_ReadBytes(DTMF_INFO_ADDR, buf, 288); // 32+16*16

    memcpy(buf, (U8 *)&g_dtmfStore, sizeof(g_dtmfStore));
    SpiFlash_EraseSector(DTMF_INFO_ADDR);

    SpiFlash_WriteBytes(DTMF_INFO_ADDR, buf, 288);
}

// 定义固定频点信息
static const U8 fixBand1[][16] = {
    {0x01, 0x01, 0x36, 0x01, 0x74, 0x01, 0x04, 0x00, 0x06, 0x00, 0x01, 0x02, 0x00, 0x02, 0x60, 0x00},
};

static const U8 fixBand2[][8] = {
    {0x00, 0x03, 0x50, 0x03, 0x90, 0x00, 0x00, 0x00},
};

// 读取对讲机 调试参数
void Flash_ReadDebugInfoData(void)
{
    U8 bandData[16];
    U8 bandData1[16];
    U8 i;

    SpiFlash_ReadBytes(RF_PWR_H_U_400_ADDR, &i, 1);
    if (i != 0xFF)
    {
        SpiFlash_ReadBytes(RF_PWR_H_U_400_ADDR, TXPWR_H_U_400, 16);
        SpiFlash_ReadBytes(RF_PWR_H_V_136_ADDR, TXPWR_H_V_136, 16);
        SpiFlash_ReadBytes(RF_PWR_H_V_200_ADDR, TXPWR_H_V_200, 16);

        SpiFlash_ReadBytes(RF_PWR_L_U_400_ADDR, TXPWR_L_U_400, 16);
        SpiFlash_ReadBytes(RF_PWR_L_V_136_ADDR, TXPWR_L_V_136, 16);
        SpiFlash_ReadBytes(RF_PWR_L_V_200_ADDR, TXPWR_L_V_200, 16);

        SpiFlash_ReadBytes(RF_SQL_TAB_ADDR, TH_SQL_TAB, 10);
        SpiFlash_ReadBytes(RF_SQL_TAB_MUTE_ADDR, TH_SQL_TAB_MUTE, 10);

        SpiFlash_ReadBytes(RF_SQL_U_400_ADDR, OFFSET_SQL_U_400, 16);
        SpiFlash_ReadBytes(RF_SQL_V_136_ADDR, OFFSET_SQL_V_136, 16);
        SpiFlash_ReadBytes(RF_SQL_V_200_ADDR, OFFSET_SQL_V_200, 16);
        SpiFlash_ReadBytes(RF_SQL_U_350_ADDR, OFFSET_SQL_U_350, 16);

        SpiFlash_ReadBytes(RF_MODULATION_ADDR, OFFSET_MODULATION, 16);

        // SpiFlash_ReadBytes(DEV_BATT_ADDR, battery.voltList, BAT_LEVEL_NUM);
    }

    // 读取机型码
    Flash_ReadRfModelType();
    g_sysRunPara.moduleType = g_rfModel.modelType;

    if (g_sysRunPara.moduleType >= 0x30 && g_sysRunPara.moduleType <= 0x35)
    {
        g_sysRunPara.moduleType &= 0x0F;
    }
    else
    {
        g_sysRunPara.moduleType = 0;
    }
    memcpy(bandData, fixBand1[0], 16);
    memcpy(bandData1, fixBand2[0], 8);

    g_sysRunPara.rfTxFlag.txEnable[0] = bandData[0];
    g_sysRunPara.rfTxFlag.txEnable[1] = bandData[5];
    g_sysRunPara.rfTxFlag.txEnable[2] = bandData[10];

    // 计算频段范围
    for (i = 0; i < 16; i++)
    {
        bandData[i] = changeHexToInt(bandData[i]);
    }
    // U段频率范围
    g_bandRang.bandFreq.vhfL = bandData[1] * 1000 + bandData[2] * 10;
    g_bandRang.bandFreq.vhfH = bandData[3] * 1000 + bandData[4] * 10;
    g_bandRang.bandFreq.freqVL = g_bandRang.bandFreq.vhfL * 10000;
    g_bandRang.bandFreq.freqVH = g_bandRang.bandFreq.vhfH * 10000;

    // V段频率范围
    g_bandRang.bandFreq.uhfL = bandData[6] * 1000 + bandData[7] * 10;
    g_bandRang.bandFreq.uhfH = bandData[8] * 1000 + bandData[9] * 10;
    g_bandRang.bandFreq.freqUL = g_bandRang.bandFreq.uhfL * 10000;
    g_bandRang.bandFreq.freqUH = g_bandRang.bandFreq.uhfH * 10000;

    // 200M频率范围
    g_bandRang.bandFreq.vhf2L = bandData[11] * 1000 + bandData[12] * 10;
    g_bandRang.bandFreq.vhf2H = bandData[13] * 1000 + bandData[14] * 10;
    g_bandRang.bandFreq.freqV2L = g_bandRang.bandFreq.vhf2L * 10000;
    g_bandRang.bandFreq.freqV2H = g_bandRang.bandFreq.vhf2H * 10000;

    // 350M频率范围
    for (i = 0; i < 8; i++)
    {
        bandData1[i] = changeHexToInt(bandData1[i]);
    }
    g_bandRang.bandFreq.B350ML = bandData1[1] * 1000 + bandData1[2] * 10;
    g_bandRang.bandFreq.B350MH = bandData1[3] * 1000 + bandData1[4] * 10;
    g_bandRang.bandFreq.freq350ML = g_bandRang.bandFreq.B350ML * 10000;
    g_bandRang.bandFreq.freq350MH = g_bandRang.bandFreq.B350MH * 10000;
}

// 保存发射允许和频段选择功能
// 预留14Byte + 2Byte(CheckSum)
//(4K - 32Byte) / 16 = 254  衰减次数: 254
//  前16字节用于衰减算法  实际使用61bit
void Flash_SaveRfModelType(void)
{
    U16 addrOffset;
    U32 logicAddr;
    U16 checkSum;
    U8 buf[16] = {0x00};

    addrOffset = Flash_GetLogicAddrShift(RF_MODEL_ADDR, 32);

    // 提取数据
    memcpy(buf, (U8 *)&g_rfModel.modelType, 5);

    // 计算CRC校验
    checkSum = CRC_ValidationCalc(buf, 14);
    memcpy(buf + 14, (U8 *)&checkSum, 2);

    if (addrOffset < 253)
    {
        logicAddr = (addrOffset + 1) * 16 + RF_MODEL_ADDR + 32;
        SpiFlash_WriteBytes(logicAddr, buf, 16);
        // 标记地址工作块
        U8 dataMap = OFF_BYTE((addrOffset + 1) % 8);
        SpiFlash_WriteBytes((RF_MODEL_ADDR + (addrOffset / 8)), &dataMap, 1);
    }
    else
    { // 整块擦除后写入
        SpiFlash_EraseSector(RF_MODEL_ADDR);
        SpiFlash_WriteBytes((RF_MODEL_ADDR + 32), buf, 16);
    }
}

// 读取对讲机发射允许和频段选择功能
// 预留14Byte + 2Byte(CheckSum)
//(4K - 32Byte) / 16 = 254  衰减次数: 254
//  前16字节用于衰减算法  实际使用61bit
void Flash_ReadRfModelType(void)
{
    U16 addrOffset;
    U32 logicAddr;
    U16 checkSum;
    U8 buf[16] = {0x00};

    addrOffset = Flash_GetLogicAddrShift(RF_MODEL_ADDR, 32);
    if (addrOffset < 254)
    {
        logicAddr = (addrOffset * 16) + RF_MODEL_ADDR + 32;
        SpiFlash_ReadBytes(logicAddr, buf, 16);

        memcpy((U8 *)&checkSum, &buf[14], 2);
        if (checkSum == CRC_ValidationCalc(buf, 14))
        { // 数据校验正确
            memcpy((U8 *)&g_rfModel.modelType, buf, 5);
            return;
        }
    }
    // 默认工厂模式，同时发射都不允许
    memset((U8 *)&g_rfModel.modelType, 0x00, 5);

    // 提取数据
    memset(buf, 0x00, 16);

    // 计算CRC校验
    checkSum = CRC_ValidationCalc(buf, 14);
    memcpy(buf + 14, (U8 *)&checkSum, 2);
    SpiFlash_EraseSector(RF_MODEL_ADDR);
    SpiFlash_WriteBytes((RF_MODEL_ADDR + 32), buf, 16);
}

// 保存对讲机收音机模式信道数据
// 收音机数据64Byte + 2Byte(CheckSum)
//(4K - 16Byte) / 66 = 61  衰减次数: 61
//  前16字节用于衰减算法  实际使用120bit
extern void Flash_SaveFmData(void)
{
    U16 addrOffset;
    U32 logicAddr;
    U16 checkSum, CheckSumRd;
    U8 buf[66] = {0x00};
    U8 cmpBuf[66];

    // 读取存储位置信息
    addrOffset = Flash_GetLogicAddrShift(FM_INFO_ADDR, 8);

    // 提取数据
    memcpy(buf, (U8 *)&g_FMInform.FmCurFreq, sizeof(STR_FMINFOS));
    // 计算CRC校验
    checkSum = CRC_ValidationCalc(buf, 64);
    memcpy(&buf[64], (U8 *)&checkSum, 2);

    if (addrOffset < 60)
    {
        logicAddr = addrOffset * 66 + FM_INFO_ADDR + 16;
        // 读取当前校验和比较
        SpiFlash_ReadBytes(logicAddr + 64, (U8 *)&CheckSumRd, 2);
        if (CheckSumRd == checkSum)
        {
            SpiFlash_ReadBytes(logicAddr, cmpBuf, 64);
            if (memcmp(cmpBuf, buf, 64) == 0)
            { // 数据相同直接返回
                return;
            }
        }
        // 数据有改变才保存
        SpiFlash_WriteBytes(logicAddr + 66, buf, 66);

        // 标记地址工作块
        U8 dataMap = OFF_BYTE((addrOffset + 1) % 8);
        SpiFlash_WriteBytes((FM_INFO_ADDR + (addrOffset / 8)), &dataMap, 1);
    }
    else
    { // 擦除
        SpiFlash_EraseSector(FM_INFO_ADDR);
        SpiFlash_WriteBytes((FM_INFO_ADDR + 16), buf, 66);
    }
}

// 读取收音机当前频率，信道数据
// 收音机数据64Byte + 2Byte(CheckSum)
//(4K - 16Byte) / 66 = 61  衰减次数: 61
//  前16字节用于衰减算法  实际使用120bit
extern void Flash_ReadFmData(void)
{
    U16 addrOffset;
    U32 logicAddr;
    U16 checkSum;
    U8 buf[66] = {0x00};

    addrOffset = Flash_GetLogicAddrShift(FM_INFO_ADDR, 8);

    if (addrOffset < 61)
    {
        logicAddr = (addrOffset * 66) + FM_INFO_ADDR + 16;
        SpiFlash_ReadBytes(logicAddr, buf, 66);

        memcpy((U8 *)&checkSum, &buf[64], 2);
        if (checkSum == CRC_ValidationCalc(buf, 64))
        { // 数据校验正确
            memcpy((U8 *)&g_FMInform.FmCurFreq, buf, 64);
            return;
        }
    }

    // 数据错误，从初始化数据恢复
    g_FMInform.FmCurFreq = 889;
    g_FMInform.fmChNum = 0;
    g_FMInform.fmChVfo = VFO_MODE;

    memcpy(buf, (U8 *)&g_FMInform.FmCurFreq, sizeof(STR_FMINFOS));
    SpiFlash_EraseSector(FM_INFO_ADDR);
    SpiFlash_WriteBytes((FM_INFO_ADDR + 16), buf, 66);
}
