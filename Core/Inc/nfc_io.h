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
#include "m24sr.h"

#include "ws28xx.h"

#define NFC_WRITE 0xAC
#define NFC_READ  0xAD
#define DEFAULT_PASSWORD_INIT { \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  \
}

#define LED_BRIGHTNESS 150 //Values from 0 to 255


uint16_t NFC_IO_IsDeviceReady(uint8_t Addr, uint32_t Trials);
void NFC_IO_ReadState(uint8_t *pPinState);
void NFC_IO_RfDisable(uint8_t PinState);
void NFC_IO_Delay(uint32_t Delay);
uint8_t convert_to_NDEF(char *text, uint8_t *ndef);

void ledLight(uint8_t klik);
void resetLight();
void ledInit();

#endif /* INC_NFC_IO_H_ */
