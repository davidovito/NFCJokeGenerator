/*
 * nfc_io.c
 *
 *  Created on: Dec 8, 2024
 *      Author: david
 */


#include "i2c.h"
#include "m24sr.h"

uint8_t DefaultPassword[16] = DEFAULT_PASSWORD_INIT;
uint8_t previous_state = 0;
uint8_t sample =0;
WS28XX_HandleTypeDef neopixel;



uint16_t NFC_IO_IsDeviceReady(uint8_t Addr, uint32_t Trials)
{
    HAL_StatusTypeDef status;

    // Check if the device is ready
    status = HAL_I2C_IsDeviceReady(&hi2c1, Addr, Trials, NFC_I2C_TIMEOUT);
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
    M24SR_Verify(NFC_WRITE, 0x0001, 0x10, DefaultPassword);

    // Re-select the NDEF file
    M24SR_SelectNDEFfile(NFC_WRITE, 0x0001);

    // Read the length of the NDEF file
    uint8_t buffer[2];
    M24SR_ReadBinary(NFC_WRITE, 0x00, 0x02, buffer);

    // End communication
    M24SR_Deselect(NFC_WRITE);

    sprintf(msg02, "presiel som az sem \n\r");
    USART2_PutBuffer((uint8_t *)msg02, strlen(msg02));
    LL_mDelay(50);

    return status;
}


void ledLight(uint8_t klik){
    switch(klik){
		case 1:
			resetLight(&neopixel);
			WS28XX_SetPixel_RGB(&neopixel, klik, 50, 0, 0);
			WS28XX_Update(&neopixel);
			break;
		case 2:
			resetLight(&neopixel);
			WS28XX_SetPixel_RGB(&neopixel, klik, 50, 25, 0);
			WS28XX_Update(&neopixel);
			break;
		case 3:
			resetLight(&neopixel);
			WS28XX_SetPixel_RGB(&neopixel, klik, 50, 50, 0);
			WS28XX_Update(&neopixel);
			break;
		case 4:
			resetLight(&neopixel);
			WS28XX_SetPixel_RGB(&neopixel, klik, 0, 50, 0);
			WS28XX_Update(&neopixel);
			break;
		case 5:
			resetLight(&neopixel);
			WS28XX_SetPixel_RGB(&neopixel, klik, 0, 0, 50);
			WS28XX_Update(&neopixel);
			break;
		case 6:
			resetLight(&neopixel);
			WS28XX_SetPixel_RGB(&neopixel, klik, 25, 0, 50);
			WS28XX_Update(&neopixel);
			break;
		case 7:
			resetLight(&neopixel);
			WS28XX_SetPixel_RGB(&neopixel, klik, 30, 10, 50);
			WS28XX_Update(&neopixel);
        	break;
		default:
			resetLight(&neopixel);
			WS28XX_SetPixel_RGB(&neopixel, 0, 50, 50, 50);
			WS28XX_Update(&neopixel);
    }
}

void resetLight(){
	WS28XX_Init(&neopixel, &htim1, 64, TIM_CHANNEL_1,  8);
	WS28XX_SetPixel_RGB(&neopixel, 0, 0, 0, 0);
	WS28XX_SetPixel_RGB(&neopixel, 1, 0, 0, 0);
	WS28XX_SetPixel_RGB(&neopixel, 2, 0, 0, 0);
	WS28XX_SetPixel_RGB(&neopixel, 3, 0, 0, 0);
	WS28XX_SetPixel_RGB(&neopixel, 4, 0, 0, 0);
	WS28XX_SetPixel_RGB(&neopixel, 5, 0, 0, 0);
	WS28XX_SetPixel_RGB(&neopixel, 6, 0, 0, 0);
	WS28XX_SetPixel_RGB(&neopixel, 7, 0, 0, 0);
	WS28XX_Update(&neopixel);
}

void ledInit(){
	WS28XX_Init(&neopixel, &htim1, 64, TIM_CHANNEL_1,  8);
}


void ledSendAnimation(){
	resetLight();

	for(int i=8; i>=0; i--){
		WS28XX_SetPixel_RGB(&neopixel, i, rand() % LED_BRIGHTNESS, rand() % LED_BRIGHTNESS, rand() % LED_BRIGHTNESS);
		WS28XX_Update(&neopixel);
		LL_mDelay(49);
		resetLight();
	}


	for(int k=0; k<8; k++){
		WS28XX_SetPixel_RGB(&neopixel, k, rand() % LED_BRIGHTNESS, rand() % LED_BRIGHTNESS, rand() % LED_BRIGHTNESS);
		WS28XX_Update(&neopixel);
		LL_mDelay(49);
		resetLight();
	}

}
