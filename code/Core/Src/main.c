/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);




/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int TIMER_CYCLE =10;
void display7SEG(int num);
void update7SEG ( int index );
void updateClockBuffer(int hour, int minute);
void updateLEDMatrix (int index );
void updateRow(uint8_t buffer);
void updateCol(int index );

const int MAX_LED = 4;
int index_led = 0;
int led_buffer [4] = {0, 0, 0, 0};

const int MAX_LED_MATRIX = 8;
int index_led_matrix = 0;
uint8_t matrix_buffer [8] = {0xFF,0xC0,0x80,0x33,0x33,0x80,0xC0,0xFF};
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	 HAL_TIM_Base_Start_IT (& htim2 );
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


//thoi gian hien thị cua 1 den led 7 doan
int switching_time = 100;
setTimer_led_7_seg(switching_time);
//thoi gian sang cua 2 den led
int time_blinking_2_led_red = 1000;
setTimer_2_LED_RED(time_blinking_2_led_red);

// thoi gian khoi tao gio : phut : giay cua he thong
int hour = 7, minute = 30, second = 1;
setTimer_clock(1000);

setTimer_matrix_led(100);

  while (1)
  {
  //hien thuc 2 LED do nhap nhay moi 1 giay
	  if(timer_2_LED_RED_flag == 1){
		  setTimer_2_LED_RED(time_blinking_2_led_red);
		  HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);
	  }

	  //hien thuc 4 led7 doan lan luot sang
	  if(timer_led_7_seg_flag == 1){
		  setTimer_led_7_seg(switching_time);
			if(index_led >= MAX_LED) index_led = 0;
				update7SEG(index_led++);
	  }

	  if(timer_clock_flag == 1){
		  setTimer_clock(1000);
		  second ++;
		  if ( second >= 60) {
			  second = 0;
			  minute ++;
		  }
		  if( minute >= 60) {
			  minute = 0;
			  hour ++;
		  }
		  if( hour >=24){
			  hour = 0;
		  }
		  updateClockBuffer (hour, minute);

	  }


	  if(timer_matrix_led_flag == 1){
		  setTimer_matrix_led(500);
		  if(index_led_matrix >= 8) index_led_matrix = 0;
		  updateLEDMatrix(index_led_matrix++);

	  }

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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
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
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|DOT_Pin|LED_RED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENM0_Pin ENM1_Pin DOT_Pin LED_RED_Pin
                           EN0_Pin EN1_Pin EN2_Pin EN3_Pin
                           ENM2_Pin ENM3_Pin ENM4_Pin ENM5_Pin
                           ENM6_Pin ENM7_Pin */
  GPIO_InitStruct.Pin = ENM0_Pin|ENM1_Pin|DOT_Pin|LED_RED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SEG0_Pin SEG1_Pin SEG2_Pin ROW2_Pin
                           ROW3_Pin ROW4_Pin ROW5_Pin ROW6_Pin
                           ROW7_Pin SEG3_Pin SEG4_Pin SEG5_Pin
                           SEG6_Pin ROW0_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void display7SEG(int num){
  	switch(num){
  	case 0:
  		HAL_GPIO_WritePin(SEG0_GPIO_Port,SEG0_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG1_GPIO_Port,SEG1_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG2_GPIO_Port,SEG2_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG3_GPIO_Port,SEG3_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG4_GPIO_Port,SEG4_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG5_GPIO_Port,SEG5_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG6_GPIO_Port,SEG6_Pin, GPIO_PIN_SET);
  		break;
  	case 1:
  		HAL_GPIO_WritePin(SEG0_GPIO_Port,SEG0_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG1_GPIO_Port,SEG1_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG2_GPIO_Port,SEG2_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG3_GPIO_Port,SEG3_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG4_GPIO_Port,SEG4_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG5_GPIO_Port,SEG5_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG6_GPIO_Port,SEG6_Pin, GPIO_PIN_SET);
  		break;
  	case 2:
  		HAL_GPIO_WritePin(SEG0_GPIO_Port,SEG0_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG1_GPIO_Port,SEG1_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG2_GPIO_Port,SEG2_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG3_GPIO_Port,SEG3_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG4_GPIO_Port,SEG4_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG5_GPIO_Port,SEG5_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG6_GPIO_Port,SEG6_Pin, GPIO_PIN_RESET);
  		break;
  	case 3:
  		HAL_GPIO_WritePin(SEG0_GPIO_Port,SEG0_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG1_GPIO_Port,SEG1_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG2_GPIO_Port,SEG2_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG3_GPIO_Port,SEG3_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG4_GPIO_Port,SEG4_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG5_GPIO_Port,SEG5_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG6_GPIO_Port,SEG6_Pin, GPIO_PIN_RESET);
  		break;
  	case 4:
  		HAL_GPIO_WritePin(SEG0_GPIO_Port,SEG0_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG1_GPIO_Port,SEG1_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG2_GPIO_Port,SEG2_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG3_GPIO_Port,SEG3_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG4_GPIO_Port,SEG4_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG5_GPIO_Port,SEG5_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG6_GPIO_Port,SEG6_Pin, GPIO_PIN_RESET);
  		break;
  	case 5:
  		HAL_GPIO_WritePin(SEG0_GPIO_Port,SEG0_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG1_GPIO_Port,SEG1_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG2_GPIO_Port,SEG2_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG3_GPIO_Port,SEG3_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG4_GPIO_Port,SEG4_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG5_GPIO_Port,SEG5_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG6_GPIO_Port,SEG6_Pin, GPIO_PIN_RESET);
  		break;
  	case 6:
  		HAL_GPIO_WritePin(SEG0_GPIO_Port,SEG0_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG1_GPIO_Port,SEG1_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG2_GPIO_Port,SEG2_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG3_GPIO_Port,SEG3_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG4_GPIO_Port,SEG4_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG5_GPIO_Port,SEG5_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG6_GPIO_Port,SEG6_Pin, GPIO_PIN_RESET);
  		break;
  	case 7:
  		HAL_GPIO_WritePin(SEG0_GPIO_Port,SEG0_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG1_GPIO_Port,SEG1_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG2_GPIO_Port,SEG2_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG3_GPIO_Port,SEG3_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG4_GPIO_Port,SEG4_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG5_GPIO_Port,SEG5_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG6_GPIO_Port,SEG6_Pin, GPIO_PIN_SET);
  		break;
  	case 8:
  		HAL_GPIO_WritePin(SEG0_GPIO_Port,SEG0_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG1_GPIO_Port,SEG1_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG2_GPIO_Port,SEG2_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG3_GPIO_Port,SEG3_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG4_GPIO_Port,SEG4_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG5_GPIO_Port,SEG5_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG6_GPIO_Port,SEG6_Pin, GPIO_PIN_RESET);
  		break;
  	case 9:
  		HAL_GPIO_WritePin(SEG0_GPIO_Port,SEG0_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG1_GPIO_Port,SEG1_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG2_GPIO_Port,SEG2_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG3_GPIO_Port,SEG3_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG4_GPIO_Port,SEG4_Pin, GPIO_PIN_SET);
  		HAL_GPIO_WritePin(SEG5_GPIO_Port,SEG5_Pin, GPIO_PIN_RESET);
  		HAL_GPIO_WritePin(SEG6_GPIO_Port,SEG6_Pin, GPIO_PIN_RESET);
  		break;

  	}
  }


void update7SEG ( int index ){
	switch ( index ){
	case 0:
		// Display the first 7 SEG with led_buffer [0]
		// chung ta chi cho phep duy nhat 1 cong ENi o trang thai enable, cac cong con lai se o trang thai disable
		//trong truong hop nay chi co cong EN0 la enable
		HAL_GPIO_WritePin(EN0_GPIO_Port,EN0_Pin,RESET);
		HAL_GPIO_WritePin(EN1_GPIO_Port,EN1_Pin,SET);
		HAL_GPIO_WritePin(EN2_GPIO_Port,EN2_Pin,SET);
		HAL_GPIO_WritePin(EN3_GPIO_Port,EN3_Pin,SET);
		display7SEG(led_buffer[0]);
		break ;
	case 1:
		// Display the second 7 SEG with led_buffer [1]
		//trong truong hop nay chi co cong EN1 la enable
		HAL_GPIO_WritePin(EN0_GPIO_Port,EN0_Pin,SET);
		HAL_GPIO_WritePin(EN1_GPIO_Port,EN1_Pin,RESET);
		HAL_GPIO_WritePin(EN2_GPIO_Port,EN2_Pin,SET);
		HAL_GPIO_WritePin(EN3_GPIO_Port,EN3_Pin,SET);
		display7SEG(led_buffer[1]);
		break ;
	case 2:
		// Display the third 7 SEG with led_buffer [2]
		//trong truong hop nay chi co cong EN2 la enable
		HAL_GPIO_WritePin(EN0_GPIO_Port,EN0_Pin,SET);
		HAL_GPIO_WritePin(EN1_GPIO_Port,EN1_Pin,SET);
		HAL_GPIO_WritePin(EN2_GPIO_Port,EN2_Pin,RESET);
		HAL_GPIO_WritePin(EN3_GPIO_Port,EN3_Pin,SET);
		display7SEG(led_buffer[2]);
		break ;
	case 3:
		// Display the forth 7 SEG with led_buffer [3]
		//trong truong hop nay chi co cong EN0 la enable
		HAL_GPIO_WritePin(EN0_GPIO_Port,EN0_Pin,SET);
		HAL_GPIO_WritePin(EN1_GPIO_Port,EN1_Pin,SET);
		HAL_GPIO_WritePin(EN2_GPIO_Port,EN2_Pin,SET);
		HAL_GPIO_WritePin(EN3_GPIO_Port,EN3_Pin,RESET);
		display7SEG(led_buffer[3]);
		break ;
	default :
		break ;
	}
}

void updateClockBuffer(int hour, int minute){
	// neu hour < 10 thi ta se them so 0 o truoc
	// vi du hour = 9 thi buffer[0] = 0, buffer[1] = 9;
	if(hour < 10){
		led_buffer[0] = 0;
		led_buffer[1] = hour;
	}
	// neu hour >10 thi ta lay so hang chuc luu vao buffer[0] va hang don vi luu vao buffer[1]
	// vi du hour = 10 thi buffer[0] = 1, buffer[1] = 0;
	if( hour >= 10 && hour < 24){
		led_buffer[0] = hour/10;
		led_buffer[1] = hour%10;
	}

	//tuong tu voi update minute
	if(minute < 10){
			led_buffer[2] = 0;
			led_buffer[3] = minute;
		}
	if( minute >= 10 && minute < 60){
			led_buffer[2] = minute/10;
			led_buffer[3] = minute%10;
	}

}

void updateLEDMatrix (int index ){
	updateCol(index);
	updateRow(matrix_buffer[index]);
}

// updateCol se tao tin hieu ENMi = 0 voi index đuoc truyen vao
//vi du updateCol(0) se lam cho duy nhat tin hieu ENM0 = RESET
void updateCol(int index ){
	switch(index){
	case 0:
		HAL_GPIO_WritePin(ENM0_GPIO_Port,ENM0_Pin,RESET);
		HAL_GPIO_WritePin(ENM1_GPIO_Port,ENM1_Pin,SET);
		HAL_GPIO_WritePin(ENM2_GPIO_Port,ENM2_Pin,SET);
		HAL_GPIO_WritePin(ENM3_GPIO_Port,ENM3_Pin,SET);
		HAL_GPIO_WritePin(ENM4_GPIO_Port,ENM4_Pin,SET);
		HAL_GPIO_WritePin(ENM5_GPIO_Port,ENM5_Pin,SET);
		HAL_GPIO_WritePin(ENM6_GPIO_Port,ENM6_Pin,SET);
		HAL_GPIO_WritePin(ENM7_GPIO_Port,ENM7_Pin,SET);

		break;
	case 1:
		HAL_GPIO_WritePin(ENM0_GPIO_Port,ENM0_Pin,SET);
		HAL_GPIO_WritePin(ENM1_GPIO_Port,ENM1_Pin,RESET);
		HAL_GPIO_WritePin(ENM2_GPIO_Port,ENM2_Pin,SET);
		HAL_GPIO_WritePin(ENM3_GPIO_Port,ENM3_Pin,SET);
		HAL_GPIO_WritePin(ENM4_GPIO_Port,ENM4_Pin,SET);
		HAL_GPIO_WritePin(ENM5_GPIO_Port,ENM5_Pin,SET);
		HAL_GPIO_WritePin(ENM6_GPIO_Port,ENM6_Pin,SET);
		HAL_GPIO_WritePin(ENM7_GPIO_Port,ENM7_Pin,SET);
		break;
	case 2:
		HAL_GPIO_WritePin(ENM0_GPIO_Port,ENM0_Pin,SET);
		HAL_GPIO_WritePin(ENM1_GPIO_Port,ENM1_Pin,SET);
		HAL_GPIO_WritePin(ENM2_GPIO_Port,ENM2_Pin,RESET);
		HAL_GPIO_WritePin(ENM3_GPIO_Port,ENM3_Pin,SET);
		HAL_GPIO_WritePin(ENM4_GPIO_Port,ENM4_Pin,SET);
		HAL_GPIO_WritePin(ENM5_GPIO_Port,ENM5_Pin,SET);
		HAL_GPIO_WritePin(ENM6_GPIO_Port,ENM6_Pin,SET);
		HAL_GPIO_WritePin(ENM7_GPIO_Port,ENM7_Pin,SET);
		break;
	case 3:
		HAL_GPIO_WritePin(ENM0_GPIO_Port,ENM0_Pin,SET);
		HAL_GPIO_WritePin(ENM1_GPIO_Port,ENM1_Pin,SET);
		HAL_GPIO_WritePin(ENM2_GPIO_Port,ENM2_Pin,SET);
		HAL_GPIO_WritePin(ENM3_GPIO_Port,ENM3_Pin,RESET);
		HAL_GPIO_WritePin(ENM4_GPIO_Port,ENM4_Pin,SET);
		HAL_GPIO_WritePin(ENM5_GPIO_Port,ENM5_Pin,SET);
		HAL_GPIO_WritePin(ENM6_GPIO_Port,ENM6_Pin,SET);
		HAL_GPIO_WritePin(ENM7_GPIO_Port,ENM7_Pin,SET);
		break;
	case 4:
		HAL_GPIO_WritePin(ENM0_GPIO_Port,ENM0_Pin,SET);
		HAL_GPIO_WritePin(ENM1_GPIO_Port,ENM1_Pin,SET);
		HAL_GPIO_WritePin(ENM2_GPIO_Port,ENM2_Pin,SET);
		HAL_GPIO_WritePin(ENM3_GPIO_Port,ENM3_Pin,SET);
		HAL_GPIO_WritePin(ENM4_GPIO_Port,ENM4_Pin,RESET);
		HAL_GPIO_WritePin(ENM5_GPIO_Port,ENM5_Pin,SET);
		HAL_GPIO_WritePin(ENM6_GPIO_Port,ENM6_Pin,SET);
		HAL_GPIO_WritePin(ENM7_GPIO_Port,ENM7_Pin,SET);
		break;
	case 5:
		HAL_GPIO_WritePin(ENM0_GPIO_Port,ENM0_Pin,SET);
		HAL_GPIO_WritePin(ENM1_GPIO_Port,ENM1_Pin,SET);
		HAL_GPIO_WritePin(ENM2_GPIO_Port,ENM2_Pin,SET);
		HAL_GPIO_WritePin(ENM3_GPIO_Port,ENM3_Pin,SET);
		HAL_GPIO_WritePin(ENM4_GPIO_Port,ENM4_Pin,SET);
		HAL_GPIO_WritePin(ENM5_GPIO_Port,ENM5_Pin,RESET);
		HAL_GPIO_WritePin(ENM6_GPIO_Port,ENM6_Pin,SET);
		HAL_GPIO_WritePin(ENM7_GPIO_Port,ENM7_Pin,SET);
		break;
	case 6:
		HAL_GPIO_WritePin(ENM0_GPIO_Port,ENM0_Pin,SET);
		HAL_GPIO_WritePin(ENM1_GPIO_Port,ENM1_Pin,SET);
		HAL_GPIO_WritePin(ENM2_GPIO_Port,ENM2_Pin,SET);
		HAL_GPIO_WritePin(ENM3_GPIO_Port,ENM3_Pin,SET);
		HAL_GPIO_WritePin(ENM4_GPIO_Port,ENM4_Pin,SET);
		HAL_GPIO_WritePin(ENM5_GPIO_Port,ENM5_Pin,SET);
		HAL_GPIO_WritePin(ENM6_GPIO_Port,ENM6_Pin,RESET);
		HAL_GPIO_WritePin(ENM7_GPIO_Port,ENM7_Pin,SET);
		break;
	case 7:
		HAL_GPIO_WritePin(ENM0_GPIO_Port,ENM0_Pin,SET);
		HAL_GPIO_WritePin(ENM1_GPIO_Port,ENM1_Pin,SET);
		HAL_GPIO_WritePin(ENM2_GPIO_Port,ENM2_Pin,SET);
		HAL_GPIO_WritePin(ENM3_GPIO_Port,ENM3_Pin,SET);
		HAL_GPIO_WritePin(ENM4_GPIO_Port,ENM4_Pin,SET);
		HAL_GPIO_WritePin(ENM5_GPIO_Port,ENM5_Pin,SET);
		HAL_GPIO_WritePin(ENM6_GPIO_Port,ENM6_Pin,SET);
		HAL_GPIO_WritePin(ENM7_GPIO_Port,ENM7_Pin,RESET);
		break;
	default:
		break;
	}





}
// ham updateRow se nhan gia tri cua buffer 8 bit voi index tuong ung trong matrix buffer va se tach tung bit cua buffer de cho ra cac output tuong ung
void updateRow(uint8_t buffer){
	// o day buffer co dang chuoi 8 bit [0:7]
	//de co the lay duoc tung bit cua buffer ta se dung cac phep and voi cac so phu hop
	// vi du de lay duoc bit thu 6 cua buffer 0010_00_1_1 thi ta and buffer cho  0x02
	// 0010_00_1_1 & 0000_00_1_0 = 0000_00_1_0
	// sau do ta dich phai de dua ket qua vua thu duoc thanh 0000_000X tuong duong với X = 0 or 1 se la tin hieu cua ROW
	uint8_t pin[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	pin[7] = (buffer & 0x01 );
	pin[6] = (buffer & 0x02 ) >> 1;
	pin[5] = (buffer & 0x04 ) >> 2;
	pin[4] = (buffer & 0x08 ) >> 3;
	pin[3] = (buffer & 0x10 ) >> 4;
	pin[2] = (buffer & 0x20 ) >> 5;
	pin[1] = (buffer & 0x40 ) >> 6;
	pin[0] = (buffer & 0x80 ) >> 7;
	//Dieu chinh tin hieu cua Row
	HAL_GPIO_WritePin(ROW0_GPIO_Port, ROW0_Pin, pin[0]);
	HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, pin[1]);
	HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, pin[2]);
	HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, pin[3]);
	HAL_GPIO_WritePin(ROW4_GPIO_Port, ROW4_Pin, pin[4]);
	HAL_GPIO_WritePin(ROW6_GPIO_Port, ROW5_Pin, pin[5]);
	HAL_GPIO_WritePin(ROW6_GPIO_Port, ROW6_Pin, pin[6]);
	HAL_GPIO_WritePin(ROW7_GPIO_Port, ROW7_Pin, pin[7]);
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	HAL_GPIO_TogglePin ( LED_RED_GPIO_Port , LED_RED_Pin );
	timer_run();
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
