#ifndef _RADIODATASTORAGE_H
#define _RADIODATASTORAGE_H

typedef enum
{
    MODIFY,
    ADD,
    DELET
} ENUM_CH_MODIFY_TYPE;

extern U8 g_VfoBuf[66];
extern U8 g_powerOnMsg[17];
extern STR_RF_MODEL g_rfModel;
extern STR_BAND g_bandRang;

void Flash_ReadVfoData(U8 workAB);
void Flash_SaveVfoData(U8 workAB);

void Flash_ModifyChannelData(U16 channelNum, const U8 *chData, const U8 *chName);
void Flash_SaveChannelData(U16 channelNum, const U8 *chData, const U8 *chName);
void Flash_DeleteChannelData(U16 channelNum);

void Flash_SaveRadioInfoData(void);
void Flash_ReadRadioInfoData(void);
void Flash_ReadDebugInfoData(void);

void Flash_SaveSystemRunData(void);
void Flash_ReadSystemRunData(void);

extern void Flash_SaveDtmfInfo(void);

void Flash_SaveRfModelType(void);
void Flash_ReadRfModelType(void);

extern void Flash_ReadFmData(void);
extern void Flash_SaveFmData(void);

#endif
