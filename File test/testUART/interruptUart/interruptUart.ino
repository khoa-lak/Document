//#include <stm32f1xx_hal.h>
//#define UART1_TX GPIO_PIN_9
//#define UART1_RX GPIO_PIN_10
//#define UART1_PORT GPIOA
//
//// DMA Rx buffer (sized for ~300-byte JSON message)
//#define DMA_RX_BUFFER_SIZE 512  // Increased to fit message; adjust based on RAM
//uint8_t dmaRxBuffer[DMA_RX_BUFFER_SIZE];
//
//// Buffer for null-terminated message string
//char msg[DMA_RX_BUFFER_SIZE + 1];  // +1 for '\0'
//
//// Flag to indicate new complete message has arrived (volatile for interrupt safety)
//volatile bool newDataAvailable = false;
//volatile uint32_t receivedLength = 0;  // Length of received message on idle
//
//// UART and DMA handles
//UART_HandleTypeDef huart1;
//DMA_HandleTypeDef hdma_usart1_rx;
//
//void setup() {
//  // Enable clocks
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  __HAL_RCC_USART1_CLK_ENABLE();
//  __HAL_RCC_DMA1_CLK_ENABLE();
//
//  // Optional: Setup LED pin for data indication (e.g., PB5 as output)
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//  GPIO_InitTypeDef GPIO_InitStruct_LED = {0};
//  GPIO_InitStruct_LED.Pin = GPIO_PIN_5;
//  GPIO_InitStruct_LED.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct_LED.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_LED);
//  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // LED off initially
//
//  // Configure UART1 pins
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  GPIO_InitStruct.Pin = UART1_TX | UART1_RX;
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(UART1_PORT, &GPIO_InitStruct);
//
//  // UART1 config (115200 baud, 8N1)
//  huart1.Instance = USART1;
//  huart1.Init.BaudRate = 115200;
//  huart1.Init.WordLength = UART_WORDLENGTH_8B;
//  huart1.Init.StopBits = UART_STOPBITS_1;
//  huart1.Init.Parity = UART_PARITY_NONE;
//  huart1.Init.Mode = UART_MODE_TX_RX;
//  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  HAL_UART_Init(&huart1);
//
//  // DMA config for UART1 Rx (DMA1 Channel5)
//  hdma_usart1_rx.Instance = DMA1_Channel5;
//  hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//  hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//  hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
//  hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//  hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//  hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;  // Circular for continuous Rx
//  hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
//  HAL_DMA_Init(&hdma_usart1_rx);
//
//  // Link DMA to UART1 Rx
//  __HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx);
//
//  // Enable DMA interrupts
//  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
//
//  // Enable UART1 interrupt for IDLE and error detection
//  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(USART1_IRQn);
//
//  // Enable IDLE line interrupt
//  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
//
//  // Start DMA Rx (circular mode)
//  HAL_UART_Receive_DMA(&huart1, dmaRxBuffer, DMA_RX_BUFFER_SIZE);
//
//  // Send startup message (using Tx polling for simplicity)
//  const char *startup_msg = "UART1 DMA Rx with Idle Detection Started\r\n";
//  HAL_UART_Transmit(&huart1, (uint8_t *)startup_msg, strlen(startup_msg), 100);
//}
//
//void loop() {
//  // Example long-running while loop (infinite for demonstration)
//  while (true) {
//    // Your time-consuming code here
//
//    // Check if a complete message has arrived (via idle detection)
//    if (newDataAvailable) {
//      // 'msg' contains the full null-terminated message
//      // Example: Echo the entire received message
//      HAL_UART_Transmit(&huart1, (uint8_t *)msg, receivedLength, 100);
//
//      // Toggle LED to indicate message received
//      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
//
//      // Optional: Parse JSON or process 'msg' here
//      // e.g., Use ArduinoJson: StaticJsonDocument<512> doc; deserializeJson(doc, msg);
//
//      newDataAvailable = false;  // Reset flag
//    }
//
//    delay(100);  // Simulate work
//  }
//}
//
//// UART Rx callback for IDLE detection (called by HAL_UART_IRQHandler on IDLE event)
//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
//  if (huart->Instance == USART1) {
//    // IDLE detected: Data transmission has stopped (end of message)
//    receivedLength = Size;
//
//    // Copy received data to 'msg' and null-terminate
//    memcpy(msg, dmaRxBuffer, receivedLength);
//    msg[receivedLength] = '\0';  // Null-terminate for char* string
//
//    // Set flag to indicate complete message received
//    newDataAvailable = true;
//
//    // Restart DMA for next message
//    HAL_UART_Receive_DMA(&huart1, dmaRxBuffer, DMA_RX_BUFFER_SIZE);
//  }
//}
//
////// UART Rx complete callback (buffer full - optional for overflow handling)
////void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
////  if (huart->Instance == USART1) {
////    // Buffer full: Process/echo (though idle will handle most cases)
////    HAL_UART_Transmit(&huart1, dmaRxBuffer, DMA_RX_BUFFER_SIZE, 100);
////  }
////}
////
////// UART Rx half-complete callback (half buffer filled - optional)
////void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
////  if (huart->Instance == USART1) {
////    // Half buffer: Process/echo if needed
////    HAL_UART_Transmit(&huart1, dmaRxBuffer, DMA_RX_BUFFER_SIZE / 2, 100);
////  }
////}
////
////// UART Error callback (optional, for robustness)
////void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
////  if (huart->Instance == USART1) {
////    // Handle errors (e.g., overrun, framing)
////    uint32_t error = HAL_UART_GetError(huart);
////    if (error & HAL_UART_ERROR_ORE) {
////      __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_ORE); // Clear overrun
////    }
////    // Restart DMA on error
////    HAL_UART_Receive_DMA(&huart1, dmaRxBuffer, DMA_RX_BUFFER_SIZE);
////  }
////}
//
//// DMA1_Channel5 IRQ Handler (required for DMA interrupts)
//extern "C" void DMA1_Channel5_IRQHandler(void) {
//  HAL_DMA_IRQHandler(&hdma_usart1_rx);
//}

// UART1 DMA Rx Code for STM32F103RCT6
// Using STM32Duino in Arduino IDE (HAL-based)

#include <stm32f1xx_hal.h>

// UART1 pins (PA9 = TX, PA10 = RX)
#define UART1_TX GPIO_PIN_9
#define UART1_RX GPIO_PIN_10
#define UART1_PORT GPIOA

// DMA Rx buffer
#define DMA_RX_BUFFER_SIZE 200  // Adjust as needed
uint8_t dmaRxBuffer[DMA_RX_BUFFER_SIZE];
// Flag to indicate new complete message has arrived (volatile for interrupt safety)
volatile bool newDataAvailable = false;
volatile uint32_t receivedLength = 0;  // Length of received message on idle
// UART and DMA handles
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

void setup() {
  // Enable clocks
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  // Configure UART1 pins
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = UART1_TX | UART1_RX;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(UART1_PORT, &GPIO_InitStruct);

  // UART1 config (115200 baud, 8N1)
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  HAL_UART_Init(&huart1);

  // DMA config for UART1 Rx (DMA1 Channel5)
  hdma_usart1_rx.Instance = DMA1_Channel5;
  hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;  // Circular for continuous Rx
  hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
  HAL_DMA_Init(&hdma_usart1_rx);

  // Link DMA to UART1 Rx
  __HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx);

  // Enable DMA interrupts
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  // Enable UART1 interrupt for IDLE detection
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  // Enable IDLE line interrupt
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  // Start DMA Rx (circular mode)
  HAL_UART_Receive_DMA(&huart1, dmaRxBuffer, DMA_RX_BUFFER_SIZE);

  // Send startup message (using Tx polling for simplicity)
  const char *msg = "UART1 DMA Rx Example Started\r\n";
  HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
}

void loop() {
  // Example long-running while loop (infinite for demonstration)
  while (true) {
    // Your time-consuming code here

    // Check if a complete message has arrived (via idle detection)
    if (newDataAvailable) {
      // Process the full message (e.g., parse JSON from dmaRxBuffer[0 to receivedLength-1])
      // Example: Echo the entire received message
      //HAL_UART_Transmit(&huart1, dmaRxBuffer, receivedLength, 100);
      Serial.println((char*)dmaRxBuffer);
      newDataAvailable = false;  // Reset flag
    }

    delay(100);  // Simulate work
  }
}

// UART1 IRQ Handler (for IDLE detection)
extern "C" void USART1_IRQHandler(void) {
  uint32_t tmp_flag = 0, tmp_it_source = 0;

  tmp_flag = __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE);
  tmp_it_source = __HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_IDLE);

  // If IDLE flag is set, data transmission has stopped (end of message)
  if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
    // Clear IDLE flag by reading SR and DR
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);

    // Stop DMA to calculate received length
    HAL_UART_DMAStop(&huart1);

    // Calculate received bytes (DMA_RX_BUFFER_SIZE - remaining transfers)
    receivedLength = DMA_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

    // Set flag to indicate complete message received
    newDataAvailable = true;

    // Restart DMA for next message
    HAL_UART_Receive_DMA(&huart1, dmaRxBuffer, DMA_RX_BUFFER_SIZE);
  }
}

// DMA Rx complete callback (buffer full - optional for overflow handling)
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//  if (huart->Instance == USART1) {
//    // Buffer full: Process/echo (though idle will handle most cases)
//    HAL_UART_Transmit(&huart1, dmaRxBuffer, DMA_RX_BUFFER_SIZE, 100);
//
//  }
//}

//// DMA Rx half-complete callback (half buffer filled - optional)
//void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
//  if (huart->Instance == USART1) {
//    // Half buffer: Process/echo if needed
//    HAL_UART_Transmit(&huart1, dmaRxBuffer, DMA_RX_BUFFER_SIZE / 2, 100);
//  }
//}

// DMA1_Channel5 IRQ Handler (required for DMA interrupts)
extern "C" void DMA1_Channel5_IRQHandler(void) {
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
}
