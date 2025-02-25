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
//#include "flash.h"
#include "flash2.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define FLASH_ADDR_PAGE_126 ((uint32_t)0x0801F810)
//#define FLASH_ADDR_PAGE_127 ((uint32_t)0x0801FC00)
//#define FLASH_USER_START_ADDR FLASH_ADDR_PAGE_126
//#define FLASH_USER_END_ADDR 	FLASH_ADDR_PAGE_127 + FLASH_PAGE_SIZE


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//uint32_t startPage = FLASH_USER_START_ADDR;
//uint32_t endPage   = FLASH_USER_END_ADDR;
//uint32_t dataRead;
//char data[50] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char *data = 		":020000040800F2"
								":1020000070060020892100080B2B0008912A000887"
								":10201000092B00088B220008672B00080000000035"
								":1020000070060020892100080B2B0008912A000887"
								":10201000092B00088B220008672B00080000000035"
								":1020000070060020892100080B2B0008912A000887"
								":10201000092B00088B220008672B00080000000035"
								":1020000070060020892100080B2B0008912A000887"
								":10201000092B00088B220008672B00080000000035"
								":1020000070060020892100080B2B0008912A000887"
								":10201000092B00088B220008672B00080000000035";
uint32_t data2[] = {0xFFFFFFFF,0x12345678,0x23456789,0x34567890,0x45678901,0x56789012,0x67890123,0x78901234,0x89012345};
uint32_t Rx_Data[30];
char string[100];
int number = 123;
float val = 123.456;
float RxVal;

void FLASH_WritePage(uint32_t startPage, uint32_t endPage, uint32_t data32){
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInit;
	EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInit.PageAddress = startPage;
	EraseInit.NbPages = 1;
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&EraseInit, &PageError);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,startPage,data32);
	//HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,startPage+4,0x09876543);
	HAL_FLASH_Lock();
}
uint32_t FLASH_ReadPage(uint32_t addr){
	uint32_t data = *(__IO uint32_t *)(addr);
	return data;
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
  /* USER CODE BEGIN 2 */
//	Flash_ProgramPage("Second data written to flash",FLASH_ADDR_PAGE_126,FLASH_ADDR_PAGE_127);
//	Flash_ReadChar(data,FLASH_ADDR_PAGE_126,FLASH_ADDR_PAGE_127);
//	HAL_Delay(2000);
//	Flash_ProgramPage("Old data",FLASH_ADDR_PAGE_126,FLASH_ADDR_PAGE_127);
//	Flash_ReadChar(data,FLASH_ADDR_PAGE_126,FLASH_ADDR_PAGE_127);
//	HAL_Delay(2000);
//	Flash_ProgramPage("o xe ke",DATA_START_ADDRESS,LENGTH_START_ADDRESS);
//	Flash_ReadChar(data,DATA_START_ADDRESS,LENGTH_START_ADDRESS);
//	HAL_Delay(2000);
//	FLASH_WritePage(FLASH_USER_START_ADDR, FLASH_USER_END_ADDR, 12345678);
//	HAL_Delay(2000);
//	FLASH_WritePage(FLASH_USER_START_ADDR, FLASH_USER_END_ADDR, 13245768);
	//dataRead = FLASH_ReadPage(FLASH_USER_START_ADDR);
	
//	Flash_Write_Data(0x08004410 , (uint32_t *)data2, 9);
//  Flash_Read_Data(0x08004410 , Rx_Data, 10);


  int numofwords = (strlen(data)/4)+((strlen(data)%4)!=0);
  Flash_Write_Data(0x08004810 , (uint32_t *)data, numofwords);
  Flash_Read_Data(0x08004810 , Rx_Data, numofwords);
  Convert_To_Str(Rx_Data, string);


//  Flash_Write_NUM(0x08005C10, number);
//  RxVal = Flash_Read_NUM(0x08005C10);

//  Flash_Write_NUM(0x08012000, val);
//  RxVal = Flash_Read_NUM(0x08012000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		Flash_Read_Data(0x08004810 , Rx_Data, numofwords);
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

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
