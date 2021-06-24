#ifndef __BSP_IIC_MEM_H__
#define __BSP_IIC_MEM_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "../../../common/inc/common/device.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_ll_dma.h"

    DEVICE_STATUS i2c_device_create(I2CDevice *device, I2C_HandleTypeDef *instance, uint16_t dmaThershold);

#ifdef __cplusplus
}
#endif

#endif // __BSP_IIC_MEM_H__