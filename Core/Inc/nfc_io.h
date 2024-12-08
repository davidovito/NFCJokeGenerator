/*
 * nfc_io.h
 *
 *  Created on: Dec 8, 2024
 *      Author: david
 */

#ifndef INC_NFC_IO_H_
#define INC_NFC_IO_H_


#include "stdint.h"
#include "stm32f3xx_ll_i2c.h"
#include "stm32f3xx_ll_gpio.h"

#define NFC_WRITE 0xAC
#define NFC_READ  0xAD


void NFC_IO_Init(uint8_t GpoIrqEnable);
void NFC_IO_DeInit(void);
uint16_t NFC_IO_ReadMultiple(uint8_t Addr, uint8_t *pBuffer, uint8_t Length);
uint16_t NFC_IO_WriteMultiple(uint8_t Addr, uint8_t *pBuffer, uint8_t Length);
uint16_t NFC_IO_IsDeviceReady(uint8_t Addr, uint32_t Trials);
void NFC_IO_ReadState(uint8_t *pPinState);
void NFC_IO_RfDisable(uint8_t PinState);
void NFC_IO_Delay(uint32_t Delay);


#endif /* INC_NFC_IO_H_ */
