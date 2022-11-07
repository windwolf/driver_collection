#ifndef ___BSP_SD_H__
#define ___BSP_SD_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "../../../common/inc/common/device.h"
#include "../../../common/inc/common/block.h"
#include "stm32h7xx_hal.h"

    OP_RESULT sd_device_create(SdDevice *device, SD_HandleTypeDef *instance, uint16_t dmaThershold);
    OP_RESULT sd_device_block_create(SdDevice *device, Block *block, Buffer8 buffer);
#ifdef __cplusplus
}
#endif

#endif // ___BSP_SD_H__