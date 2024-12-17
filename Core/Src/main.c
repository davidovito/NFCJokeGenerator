/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ws28xx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t led = 0;
uint8_t numOfSamps = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
WS28XX_HandleTypeDef neopixel;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  WS28XX_Init(&neopixel, &htim1, 64, TIM_CHANNEL_1,  8);
  //  M24SR_Init(NFC_WRITE, M24SR_GPO_POLLING);
//  M24SR_ManageRFGPO(NFC_WRITE, 1);

  WS28XX_SetPixel_RGB(&neopixel, 0, 50, 50, 50);
  WS28XX_Update(&neopixel);

  sdCardInit();
  char joke[250];
  char msg[250];

  readRandomJokes(1, joke);


  //sprintf(msg, "%s\r\n", joke);
  USART2_PutBuffer(joke, strlen(joke));

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_16);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(64000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
  LL_RCC_SetTIMClockSource(LL_RCC_TIM1_CLKSOURCE_PCLK2);
}

/* USER CODE BEGIN 4 */
/***************************************Tlačidlo na zmenu režimu****************************************/
void ledCount(uint32_t stateOfButton){
	if (edgeDetect(stateOfButton, 10) == RISE){
		led++;
		if(led >= 8){
			led = 0;
		}
		ledLight(led);
    }
}

/***************************************Detekcia nabeznej hrany************************************/
enum EDGE_TYPE edgeDetect(uint16_t pin_state, uint16_t samples){
	if (pin_state == 1) {
    	numOfSamps++;
    	if (numOfSamps >= samples) {
    		numOfSamps = 0;
            return RISE;
        }
	}
    else if (pin_state == 0) {
    	numOfSamps = 0;
    }
	return NONE;
}

/***************************************Reset led svetiel******************************************/
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

/***************************************Ktora led ma svitit****************************************/
void ledLight(uint8_t klik){
    switch(klik){
		case 1:
			resetLight;
			WS28XX_Init(&neopixel, &htim1, 64, TIM_CHANNEL_1,  8);
			WS28XX_SetPixel_RGB(&neopixel, klik, 50, 0, 0);
			WS28XX_Update(&neopixel);
			break;
		case 2:
			resetLight;
			WS28XX_Init(&neopixel, &htim1, 64, TIM_CHANNEL_1,  8);
			WS28XX_SetPixel_RGB(&neopixel, klik, 50, 25, 0);
			WS28XX_Update(&neopixel);
			break;
		case 3:
			resetLight;
			WS28XX_Init(&neopixel, &htim1, 64, TIM_CHANNEL_1,  8);
			WS28XX_SetPixel_RGB(&neopixel, klik, 50, 50, 0);
			WS28XX_Update(&neopixel);
			break;
		case 4:
			resetLight;
			WS28XX_Init(&neopixel, &htim1, 64, TIM_CHANNEL_1,  8);
			WS28XX_SetPixel_RGB(&neopixel, klik, 0, 50, 0);
			WS28XX_Update(&neopixel);
			break;
		case 5:
			resetLight;
			WS28XX_Init(&neopixel, &htim1, 64, TIM_CHANNEL_1,  8);
			WS28XX_SetPixel_RGB(&neopixel, klik, 0, 0, 50);
			WS28XX_Update(&neopixel);
			break;
		case 6:
			resetLight;
			WS28XX_Init(&neopixel, &htim1, 64, TIM_CHANNEL_1,  8);
			WS28XX_SetPixel_RGB(&neopixel, klik, 25, 0, 50);
			WS28XX_Update(&neopixel);
			break;
		case 7:
			resetLight;
			WS28XX_Init(&neopixel, &htim1, 64, TIM_CHANNEL_1,  8);
			WS28XX_SetPixel_RGB(&neopixel, klik, 30, 10, 50);
			WS28XX_Update(&neopixel);
        	break;
		default:
			resetLight;
			WS28XX_Init(&neopixel, &htim1, 64, TIM_CHANNEL_1,  8);
			WS28XX_SetPixel_RGB(&neopixel, 0, 50, 50, 50);
			WS28XX_Update(&neopixel);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
