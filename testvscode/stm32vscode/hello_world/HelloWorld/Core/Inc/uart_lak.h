#ifndef uart_lak_h
#define uart_lak_h
#ifdef __cplusplus
extern "C"
{
#endif
#include "stm32f1xx_hal.h"
    uint8_t rx_data;
    uint8_t tx_data[20] = "STM32 Hello Man!!\r\n";
    void setupUartLak(void);
    void sendUartLak();
    void reciveUartLak();
#ifdef __cplusplus
}
#endif
#endif
