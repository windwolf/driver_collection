#ifndef ___BSP_UART_H__
#define ___BSP_UART_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "../../../common/inc/common/device.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_dma.h"

#ifdef HAL_UART_MODULE_ENABLED

    OP_RESULT usart_device_create(UartDevice *device, UART_HandleTypeDef *huart, uint16_t dmaThershold);

#endif // HAL_UART_MODULE_ENABLED

#ifdef __cplusplus
}
#endif

#endif // ___BSP_UART_H__