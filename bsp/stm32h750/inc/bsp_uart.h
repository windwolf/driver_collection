#ifndef ___BSP_UART_H__
#define ___BSP_UART_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "common/device.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_uart.h"
#include "stm32h7xx_hal_uart_ex.h"
#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_dma.h"

    DEVICE_STATUS uart_device_create(UartDevice *device, UART_HandleTypeDef *huart, uint16_t dmaThershold);

#ifdef __cplusplus
}
#endif

#endif // ___BSP_UART_H__