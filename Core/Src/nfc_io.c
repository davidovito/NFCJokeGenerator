/*
 * nfc_io.c
 *
 *  Created on: Dec 8, 2024
 *      Author: david
 */


#include "i2c.h"
#include "m24sr.h"

void NFC_IO_Init(uint8_t GpoIrqEnable){

}

void NFC_IO_DeInit(void){

}

/*
uint16_t NFC_IO_ReadMultiple(uint8_t Addr, uint8_t *pBuffer, uint16_t Length){
	if(I2C_ReadMultiByte(NFC_READ, Addr, pBuffer, Length) != 0){
		return NFC_IO_ERROR_TIMEOUT;
	}
	return NFC_IO_STATUS_SUCCESS;
}

uint16_t NFC_IO_WriteMultiple(uint8_t Addr, uint8_t *pBuffer, uint16_t Length){
	if(I2C_WriteMultiByte(NFC_WRITE, Addr, pBuffer, Length) != 0){
		return NFC_IO_ERROR_TIMEOUT;
	}
	return NFC_IO_STATUS_SUCCESS;
}

*/

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


uint8_t convert_to_NDEF(char *text, uint8_t *ndef){
	if (text == 0)
		return 1;

	size_t msg_size = strlen(text);

	uint8_t TNF_FLAGS = 0xD1;
	uint8_t type_length = 0x1;
	uint8_t payload_length = (uint8_t)(msg_size + 3);
	uint8_t type_text = 0x54;
	uint8_t status = 0x02;
	uint8_t language1 = 0x65;
	uint8_t language2 = 0x6E;

	uint8_t ndef_header[] = {TNF_FLAGS, type_length, payload_length, type_text, status, language1, language2};


    memcpy(ndef, ndef_header, sizeof(ndef_header));
    memcpy(ndef + sizeof(ndef_header), text, strlen(text));

	return 0;
}


