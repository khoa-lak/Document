#include "flash.h"
#define DATA_START_ADDRESS       ((uint32_t)0x08001C20 )  //Page 127
#define LENGTH_START_ADDRESS    ((uint32_t)0x08001C10)   //Page 126
char data[50] = {0};
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  // put your main code here, to run repeatedly:
  Flash_ProgramPage("First data written to flash", DATA_START_ADDRESS, LENGTH_START_ADDRESS);
  Flash_ReadChar(data, DATA_START_ADDRESS, LENGTH_START_ADDRESS);
  Serial.println(data);
  HAL_Delay(2000);
  Flash_ProgramPage("New data", DATA_START_ADDRESS, LENGTH_START_ADDRESS);
  Flash_ReadChar(data, DATA_START_ADDRESS, LENGTH_START_ADDRESS);
  Serial.println(data);
  while (1) {

  }
}
