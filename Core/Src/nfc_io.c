/*
 * nfc_io.c
 *
 *  Created on: Dec 8, 2024
 *      Author: david
 */


#include "i2c.h"
#include "m24sr.h"
#include "usart.h"

uint8_t DefaultPassword[16] = DEFAULT_PASSWORD_INIT;


uint16_t NFC_IO_IsDeviceReady(uint8_t Addr, uint32_t Trials)
{
    HAL_StatusTypeDef status;
    uint32_t tickstart = 0;
    uint32_t currenttick = 0;

    tickstart = HAL_GetTick();

    status = HAL_I2C_IsDeviceReady(&hi2c1, Addr, Trials, NFC_I2C_TIMEOUT);

    do
    {
        status = HAL_I2C_IsDeviceReady(&hi2c1, Addr, Trials, 1000);
        currenttick = HAL_GetTick();
    } while( ( (currenttick - tickstart) < 1500) && (status != HAL_OK) );


    if (status != HAL_OK)
    {
        return NFC_IO_ERROR_TIMEOUT;
    }

    return NFC_IO_STATUS_SUCCESS;
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
	LL_mDelay(Delay);
}


uint8_t convert_to_NDEF(char *text, uint8_t *ndef){
    char msg[100];

	if (text == 0)
		return 1;

	size_t msg_size = strlen(text)+11;
	uint8_t P1 = (uint8_t) ((msg_size & 0xFF00 )>>8);
	uint8_t P2 = (uint8_t) (msg_size & 0x00FF );

	size_t msg_last = strlen(text)+4;


	uint8_t TNF_FLAGS = 0xC1;
	uint8_t type_length = 0x1;
	uint8_t payload_length = (uint8_t)(msg_size + 3);
	uint8_t type_text = 0x54;
	uint8_t status = 0x02;
	uint8_t language1 = 0x65;
	uint8_t language2 = 0x6E;

	uint8_t ndef_header[] = {P1, P2, TNF_FLAGS, type_length,  0x0 ,0x0,0x0,msg_last, type_text, status, language1, language2};

    memcpy(ndef, ndef_header, sizeof(ndef_header));
    memcpy(ndef + sizeof(ndef_header), text, strlen(text)+1);


	return 0;
}


uint16_t Write_Joke_TO_NFC(uint8_t *ndef_message, uint16_t celkova_dlzka)
{
    char msg02[100];
    uint16_t status;

    // Start I2C communication
    M24SR_KillSession(NFC_WRITE);

    // Send SelectNDEFTagApplication command
    M24SR_SelectApplication(NFC_WRITE);

    // Select NDEF file
    status = M24SR_SelectNDEFfile(NFC_WRITE, 0x0001);

    if (status == M24SR_ERROR_TIMEOUT) {
        // If timeout occurred, print a timeout message
        sprintf(msg02, "Timeout occurred\n\r");
    } else {
        // Otherwise, print the returned status as a decimal or hex
        sprintf(msg02, "Status: %d\n\r", status);
    }

    USART2_PutBuffer((uint8_t *)msg02, strlen(msg02));
    LL_mDelay(50);

    // Verify access with password for the NDEF file
    M24SR_Verify(NFC_WRITE, 0x0002, 0x10, DefaultPassword);

    // Write the joke to the NDEF file
    M24SR_UpdateBinary(NFC_WRITE, 0x00, celkova_dlzka, ndef_message);

    // Verify access with password for the system file
    //M24SR_Verify(NFC_WRITE, 0x0001, 0x10, DefaultPassword);

    // Re-select the NDEF file
    //M24SR_SelectNDEFfile(NFC_WRITE, 0x0001);

    // Read the length of the NDEF file
    uint8_t buffer[2];
    //M24SR_ReadBinary(NFC_WRITE, 0x00, 0x02, buffer);

    // End communication
    M24SR_Deselect(NFC_WRITE);

    sprintf(msg02, "presiel som az sem \n\r");
    USART2_PutBuffer((uint8_t *)msg02, strlen(msg02));
    LL_mDelay(50);

    return status;
}


