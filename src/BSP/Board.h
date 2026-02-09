#ifndef __BOARD_H
#define __BOARD_H

extern void GpioModeSwitch(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t mode);
extern void uartSendChar(uint8_t ch);
extern uint8_t UserADC_GetValOfBatt(void);
extern uint8_t UserADC_GetValOfVox(void);
extern void Board_Init(void);

#endif
