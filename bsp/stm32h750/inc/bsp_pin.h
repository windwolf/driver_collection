#ifndef ___BSP_PIN_H__
#define ___BSP_PIN_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "../../../common/inc/common/device.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_ll_gpio.h"

    DEVICE_STATUS pin_device_create(PinDevice *device, GPIO_TypeDef *handle, uint16_t pinMask, PIN_DEVICE_STATUS_INVERSE inverse);

#ifdef __cplusplus
}
#endif

#endif // ___BSP_PIN_H__