/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include <string.h>
#include "st7735.h"
#include "fonts.h"
/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi1;
RTC_TimeTypeDef time;
RTC_DateTypeDef date;
/* USER CODE BEGIN PV */
char display_time[30];
char display_date[30];
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
#define CS_PORT GPIOB
#define CS_PIN  GPIO_PIN_6
#define DC_PORT GPIOA
#define DC_PIN  GPIO_PIN_9
#define RST_PORT GPIOC
#define RST_PIN  GPIO_PIN_13

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  ST7735_Init();
  time.Hours = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
  time.Minutes = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
  time.Seconds = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3);
  HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);

  sprintf(display_time,"%02d:%02d:%02d",time.Hours,time.Minutes,time.Hours);

  date.Year = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);
  date.Month = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR5);
  date.Date = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR6);
  HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);

 // sprintf(display_date,"%02d-%02d-%02d",date.Date,date.Month,date.Year);
   char sec_arry[2];
   char min_arry[2];
   char hrs_arry[2];
   char date_arry[2];
   char year_arry[5];

   ST7735_FillScreenFast(0xFCE0);
   ST7735_drawCircle(62, 125, 23, 0x82A3);
   ST7735_drawCircle(62, 125, 20, 0x82A3);
   ST7735_drawLine(62, 125, 49, 125, 0x82A3);
   ST7735_drawLine(62, 125, 72, 110, 0x82A3);

    while (1)
     {
       HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
       HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
       sec_arry[0]='0'+(time.Seconds/10);
       sec_arry[1]='0'+(time.Seconds%10);
       min_arry[0]='0'+(time.Minutes/10);
       min_arry[1]='0'+(time.Minutes%10);
       hrs_arry[0]='0'+(time.Hours/10);
       hrs_arry[1]='0'+(time.Hours%10);
       date_arry[0]='0'+(date.Date/10);
       date_arry[1]='0'+(date.Date%10);
       year_arry[0]='2';
       year_arry[1]='0';
       year_arry[2]='0'+(date.Year/10);
       year_arry[3]='0'+(date.Year%10);
       year_arry[4]='\0';
       char month[10];
       sprintf(month,"%02d",date.Month);
       ST7735_WriteString(34, 10, "Clock", Font_11x18, 0x82A3, 0xEF1C);

       ST7735_WriteString(20, 45, hrs_arry,Font_11x18 , 0x82A3,0xEF1C);
       ST7735_WriteString(40, 45, ":",Font_11x18 ,  0x82A3,0xEF1C);
       ST7735_WriteString(50, 45, min_arry,Font_11x18 ,  0x82A3,0xEF1C);
       ST7735_WriteString(70, 45, ":",Font_11x18 , 0x82A3,0xEF1C);
       ST7735_WriteString(80, 45, sec_arry,Font_11x18 , 0x82A3,0xEF1C);

       ST7735_WriteString(15, 80, date_arry, Font_11x18,  0x82A3,0xEF1C);
       ST7735_WriteString(35, 80, "/",Font_11x18 , 0x82A3, 0xEF1C);
       ST7735_WriteString(45, 80, month, Font_11x18,  0x82A3, 0xEF1C);
       ST7735_WriteString(65, 80, "/",Font_11x18 ,  0x82A3, 0xEF1C);
       ST7735_WriteString(75, 80, year_arry, Font_11x18, 0x82A3, 0xEF1C);

       HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, time.Hours);
       HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, time.Minutes);
       HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, time.Seconds);
       HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, date.Year);
       HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, date.Month);
       HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR6, date.Date);
       HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR7, date.WeekDay);

  }
  /* USER CODE END 3 */
}
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}
static void MX_RTC_Init(void)
{
	  /* USER CODE BEGIN RTC_Init 1 */

	  /* USER CODE END RTC_Init 1 */

	  /** Initialize RTC Only
	  */
	  hrtc.Instance = RTC;
	  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
	  if (HAL_RTC_Init(&hrtc) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  time.Hours = 0x15;
	  	  time.Minutes = 0x2B;
	  	  time.Seconds = 0x10;
	  if (HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN) != HAL_OK)
	  {
	    Error_Handler();
  }
  date.WeekDay = RTC_WEEKDAY_THURSDAY;
  date.Month = RTC_MONTH_APRIL;
  date.Date = 0xA;
  date.Year = 0x18;

  if (HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB1 PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }

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
