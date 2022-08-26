#ifndef ___BSP_UART_H__
#define ___BSP_UART_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "common/device.h"
#include "stm32g0xx_hal.h"

#ifdef HAL_UART_MODULE_ENABLED

    OP_RESULT uart_device_create(UartDevice *device, UART_HandleTypeDef *huart, uint16_t dmaThershold);

    void uart_send_byte(const char *data, uint16_t len);



#endif // HAL_UART_MODULE_ENABLED

#ifdef __cplusplus
}
#endif

#endif // ___BSP_UART_H__