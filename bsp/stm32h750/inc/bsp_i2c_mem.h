#ifndef __BSP_IIC_MEM_H__
#define __BSP_IIC_MEM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "common/device.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_hal_i2c.h"
#include "stm32h7xx_hal_i2c_ex.h"

    DEVICE_STATUS i2c_mem_device_create(I2CMemDevice *device, I2C_HandleTypeDef *instance, uint16_t dmaThershold);

#ifdef __cplusplus
}
#endif

#endif // __BSP_IIC_MEM_H__