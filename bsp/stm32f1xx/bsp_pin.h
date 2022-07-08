#ifndef ___BSP_PIN_H__
#define ___BSP_PIN_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "common/device.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_gpio.h"

#ifdef HAL_GPIO_MODULE_ENABLED

    OP_RESULT pin_device_create(PinDevice *device, GPIO_TypeDef *handle, uint16_t pinMask, PIN_DEVICE_STATUS_INVERSE inverse);

#endif // HAL_GPIO_MODULE_ENABLED

#ifdef __cplusplus
}
#endif

#endif // ___BSP_PIN_H__