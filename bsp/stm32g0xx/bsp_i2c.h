#ifndef __BSP_IIC_MEM_H__
#define __BSP_IIC_MEM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "common/device.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_dma.h"

#ifdef HAL_I2C_MODULE_ENABLED

    OP_RESULT i2c_device_create(I2CDevice *device, I2C_HandleTypeDef *instance, uint16_t dmaThershold);

#endif // HAL_I2C_MODULE_ENABLED

#ifdef __cplusplus
}
#endif

#endif // __BSP_IIC_MEM_H__