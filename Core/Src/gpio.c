/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  static uint8_t GpoIrqEnabled = 0;

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
*/
void MX_GPIO_Init(void)
{

  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(RF_DIS_GPIO_Port, RF_DIS_Pin);

  /**/
  LL_GPIO_SetOutputPin(CS_GPIO_Port, CS_Pin);

  /**/
  GPIO_InitStruct.Pin = DET_Pin|I2C_GPO_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = RF_DIS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(RF_DIS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(CS_GPIO_Port, &GPIO_InitStruct);

  /**/
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE5);

  /**/
  LL_GPIO_SetPinPull(Jokes_Button_GPIO_Port, Jokes_Button_Pin, LL_GPIO_PULL_UP);

  /**/
  LL_GPIO_SetPinMode(Jokes_Button_GPIO_Port, Jokes_Button_Pin, LL_GPIO_MODE_INPUT);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_5;
  EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI9_5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 2 */
void NFC_IO_Init(uint8_t GpoIrqEnable)
{

    GPIO_InitStruct.Pin = I2C_GPO_Pin;
    GPIO_InitStruct.Mode = GpoIrqEnable ? GPIO_MODE_IT_RISING : LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(I2C_GPO_GPIO_Port, &GPIO_InitStruct);

    if (GpoIrqEnable)
    {
        NVIC_SetPriority(EXTI9_5_IRQn, 0);
        NVIC_EnableIRQ(EXTI9_5_IRQn);
    }

    GPIO_InitStruct.Pin = RF_DIS_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    LL_GPIO_Init(RF_DIS_GPIO_Port, &GPIO_InitStruct);

    LL_GPIO_ResetOutputPin(RF_DIS_GPIO_Port, RF_DIS_Pin);

    GpoIrqEnabled = GpoIrqEnable;
}

void NFC_IO_DeInit(void)
{
    LL_GPIO_DeInit(I2C_GPO_GPIO_Port);
    LL_GPIO_DeInit(RF_DIS_GPIO_Port);

    if (GpoIrqEnabled)
    {
        NVIC_DisableIRQ(EXTI9_5_IRQn);
    }

    GpoIrqEnabled = 0;
}
/* USER CODE END 2 */
