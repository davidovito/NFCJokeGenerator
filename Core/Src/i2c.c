/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB6   ------> I2C1_SCL
  PB7   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* I2C1 DMA Init */

  /* I2C1_TX Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MDATAALIGN_BYTE);

  LL_SYSCFG_SetRemapDMA_I2C(LL_SYSCFG_I2C1TX_RMP_DMA1_CH2);

  /* I2C1 interrupt Init */
  NVIC_SetPriority(I2C1_EV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(I2C1_EV_IRQn);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_EnableAutoEndMode(I2C1);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x00201D2B;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/* USER CODE BEGIN 1 */

uint8_t I2C_WriteMultiByte(uint8_t DevAddress, uint8_t RegAddress, uint8_t *Data, uint16_t Size)
{
    while (LL_I2C_IsActiveFlag_BUSY(I2C1));

    LL_I2C_HandleTransfer(I2C1, DevAddress & 0xFE, LL_I2C_ADDRSLAVE_7BIT, Size + 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

    while (!LL_I2C_IsActiveFlag_TXIS(I2C1));
    LL_I2C_TransmitData8(I2C1, RegAddress);

    for (uint16_t i = 0; i < Size; i++)
    {
        while (!LL_I2C_IsActiveFlag_TXIS(I2C1));
        LL_I2C_TransmitData8(I2C1, Data[i]);
    }

    while (!LL_I2C_IsActiveFlag_STOP(I2C1));
    LL_I2C_ClearFlag_STOP(I2C1);

    return 0;
}


uint8_t I2C_ReadMultiByte(uint8_t DevAddress, uint8_t RegAddress, uint8_t *Data, uint16_t Size)
{
    while (LL_I2C_IsActiveFlag_BUSY(I2C1));

    LL_I2C_HandleTransfer(I2C1, DevAddress & 0xFE, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

        if (Size > 1) {
        	RegAddress |= 0x80; // Set MSB for auto-increment
        }

    LL_I2C_TransmitData8(I2C1, RegAddress);
    while (!LL_I2C_IsActiveFlag_TC(I2C1));

    LL_I2C_HandleTransfer(I2C1, DevAddress | 0x01, LL_I2C_ADDRSLAVE_7BIT, Size, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);

    for (uint16_t i = 0; i < Size; i++)
    {
        while (!LL_I2C_IsActiveFlag_RXNE(I2C1));
        Data[i] = LL_I2C_ReceiveData8(I2C1);
    }
    while (!LL_I2C_IsActiveFlag_STOP(I2C1));
    LL_I2C_ClearFlag_STOP(I2C1);

    return 0;
}
/* USER CODE END 1 */
