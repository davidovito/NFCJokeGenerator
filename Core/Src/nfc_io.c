/*
 * nfc_io.c
 *
 *  Created on: Dec 8, 2024
 *      Author: david
 */


#include "nfc_io.h"
#include "i2c.h"

void NFC_IO_Init(uint8_t GpoIrqEnable){

}

void NFC_IO_DeInit(void){

}

uint16_t NFC_IO_ReadMultiple(uint8_t Addr, uint8_t *pBuffer, uint8_t Length){
	if(I2C_ReadMultiByte(NFC_READ, Addr, pBuffer, Lenght) != 0){
		return NFC_IO_ERROR_TIMEOUT;
	}
	return NFC_IO_STATUS_SUCCESS;
}

uint16_t NFC_IO_WriteMultiple(uint8_t Addr, uint8_t *pBuffer, uint8_t Length){
	if(I2C_WriteMultiByte(NFC_WRITE, Addr, pBuffer, Lenght) != 0){
		return NFC_IO_ERROR_TIMEOUT;
	}
	return NFC_IO_STATUS_SUCCESS;
}

uint16_t NFC_IO_IsDeviceReady(uint8_t Addr, uint32_t Trials){
    for (uint32_t i = 0; i < Trials; i++)
    {
        LL_I2C_HandleTransfer(I2C1, Addr, LL_I2C_ADDRSLAVE_7BIT, 0, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

        if (LL_I2C_IsActiveFlag_NACK(I2C1))
        {
            LL_I2C_ClearFlag_NACK(I2C1);
            NFC_IO_Delay(10);
        }
        else
        {
            return NFC_IO_STATUS_SUCCESS;
        }
    }

    return NFC_IO_ERROR_TIMEOUT;
}

void NFC_IO_ReadState(uint8_t *pPinState){
    *pPinState = LL_GPIO_IsInputPinSet(GPIOA, I2C_GPO_Pin);
}

void NFC_IO_RfDisable(uint8_t PinState){
    if (PinState){
        LL_GPIO_SetOutputPin(GPIOA, RF_DIS_Pin);
    }
    else{
        LL_GPIO_ResetOutputPin(GPIOA, RF_DIS_Pin);
    }
}

void NFC_IO_Delay(uint32_t Delay){

}


