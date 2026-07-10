/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "math.h"

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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define PI 3.14159265358979323846f
#define DEG2RAD (PI / 180.0f)
#define MAX_POINTS 8


float angle = 0.0;
float rad = 0.0f;
float z_offset = -4.0; //offset po z
float mult = 70.0; //scaler
int osc = 0; //oscilator
uint8_t shape = 1; //forma
int n; //tochki


uint8_t selectPressed = 0;

const int cube_points[8][3] = {
		  {-1,-1,1},
		  {1,-1,1},
		  {1,1,1},
		  {-1,1,1},
		  {-1,-1,-1},
		  {1,-1,-1},
		  {1,1,-1},
		  {-1,1,-1}
};

const int pyramid_points[5][3] = {
		  {1,-1,-1}, // 0
		  {-1,-1,-1}, // 1
		  {-1,-1,1}, // 2
		  {1,-1,1}, // 3
		  {0,1,0}, // 4
};


static void displayCube(int points_2d[][2]){

	//CLear Screen
	ssd1306_Fill(Black);

	  //Display Cube
	  ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[1][0], points_2d[1][1], White); // 0 -1
	  ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[2][0], points_2d[2][1], White); // 1 - 2
	  ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[3][0], points_2d[3][1], White); // 2 - 3
	  ssd1306_Line(points_2d[3][0], points_2d[3][1], points_2d[0][0], points_2d[0][1], White); // 3 - 0

	  ssd1306_Line(points_2d[4][0], points_2d[4][1], points_2d[5][0], points_2d[5][1], White); // 4 - 5
	  ssd1306_Line(points_2d[5][0], points_2d[5][1], points_2d[6][0], points_2d[6][1], White); // 5 - 6
	  ssd1306_Line(points_2d[6][0], points_2d[6][1], points_2d[7][0], points_2d[7][1], White); // 6 - 7
	  ssd1306_Line(points_2d[7][0], points_2d[7][1], points_2d[4][0], points_2d[4][1], White); // 7 - 4

	  ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[4][0], points_2d[4][1], White); // 0 - 4
	  ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[5][0], points_2d[5][1], White); // 1 - 5
	  ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[6][0], points_2d[6][1], White); // 2 - 6
	  ssd1306_Line(points_2d[3][0], points_2d[3][1], points_2d[7][0], points_2d[7][1], White); // 3 - 7

	  ssd1306_UpdateScreen();

}

static void displayPyramid(int points_2d[][2]){

	//CLear Screen
	ssd1306_Fill(Black);

	  ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[1][0], points_2d[1][1], White); // 0 -1
	  ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[2][0], points_2d[2][1], White); // 1 - 2
	  ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[3][0], points_2d[3][1], White); // 2 - 3
	  ssd1306_Line(points_2d[3][0], points_2d[3][1], points_2d[0][0], points_2d[0][1], White); // 3 - 0

	  ssd1306_Line(points_2d[0][0], points_2d[0][1], points_2d[4][0], points_2d[4][1], White); // 4 - 5
	  ssd1306_Line(points_2d[1][0], points_2d[1][1], points_2d[4][0], points_2d[4][1], White); // 5 - 6
	  ssd1306_Line(points_2d[2][0], points_2d[2][1], points_2d[4][0], points_2d[4][1], White); // 6 - 7
	  ssd1306_Line(points_2d[3][0], points_2d[3][1], points_2d[4][0], points_2d[4][1], White); // 7 - 4


	  ssd1306_UpdateScreen();
}


static float sizeCalc(int osc){
	//change cube size
	return 55 + sin(osc * 0.2)*15; //50 - 70
}

static void determineAngle(float* angle, uint8_t shape){
	if(shape == 0){ //kwadrat
		  if(*angle < 90 - 5){
			*angle += 5;
		  	  }else{
		  		*angle = 0;
		  }
	}else if(shape == 1){ //piramida
		  if(*angle < 360 - 5){
			  *angle += 5;
		  	  }else{
		  		  *angle = 0;
		  }
	}

}


static void rotateXYZ(const int original_points[][3], float rotated_points[][3], float rad, float z_offset, int n){
	  //rotate x, y, z
	  	  for(int i = 0; i < n; i++){
	  		  //X value
	  		  rotated_points[i][0] = original_points[i][0] * cos(rad) - original_points[i][2] * sin(rad);

	  		  //Y val - ne se promenq, zashtoto wurtim po Y-axis
	  		  rotated_points[i][1] = original_points[i][1];

	  		  //Z val + Z_offset
	  		  rotated_points[i][2] = original_points[i][0] * sin(rad) + original_points[i][2] * cos(rad) + z_offset;
	  	  }
}


static void pointsConvert2D(int points_2d[][2], float rotated_points[][3],int n){
	  //3d to 2d
	  for(int i = 0; i < n; i++){
		  //X val = Xc = x / z
		  points_2d[i][0] = roundf(64 + rotated_points[i][0] / rotated_points[i][2] * mult); // +64 za centrirane

		  //Y val = Yc = y / z
		  points_2d[i][1] = roundf(32 + rotated_points[i][1] / rotated_points[i][2] * mult); //+32 za centrirane
	  }
}



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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  const int (*original_points)[3];
  int points_2d[MAX_POINTS][2]; //x,y - 2d
  float rotated_points[MAX_POINTS][3];//x,y,z zawurtqni w 3d

  ssd1306_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 0 && selectPressed == 0){
		selectPressed = 1;
		shape = !shape;
	}

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 1 && selectPressed == 1){
		selectPressed = 0;
	}



	if(shape == 0){
		n = 8;
		original_points = cube_points;
	}else
		if(shape == 1){
			n = 5;
			original_points = pyramid_points;
	}

	//change size
	osc++;
	mult = sizeCalc(osc);

	//determine angle
	determineAngle(&angle, shape);

	//rad calc
	rad = angle * DEG2RAD;

	//rotate XYZ
	rotateXYZ(original_points, rotated_points, rad, z_offset, n);

	//3D to 2D
	pointsConvert2D(points_2d, rotated_points, n);

	if(shape == 0){
		displayCube(points_2d);
	}
	else
		if(shape == 1){
			displayPyramid(points_2d);
		}

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00B07CB4;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
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
