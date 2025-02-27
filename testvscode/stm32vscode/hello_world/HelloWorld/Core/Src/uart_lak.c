#include "uart_lak.h"
void setupUartLak(void)
{
    return;
}
void sendUartLak()
{
    HAL_UART_Transmit(&huart1,tx_data, sizeof(tx_data), 100);
    return;
}
void reciveUartLak()
{
    HAL_UART_Receive_IT(&huart1, &rx_data, 1);
    return;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance == USART1){
        HAL_UART_Transmit(&huart1,&rx_data,sizeof(rx_data), 100);
        HAL_UART_Receive_IT(&huart1, &rx_data, 1);
    }
}