#ifndef ___BSP_SD_H__
#define ___BSP_SD_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "../../../common/inc/common/device.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_sd.h"

    DEVICE_STATUS sd_device_create(SdDevice *device, SD_HandleTypeDef *instance, uint16_t dmaThershold, uint32_t blockSize);

#ifdef __cplusplus
}
#endif

#endif // ___BSP_SD_H__