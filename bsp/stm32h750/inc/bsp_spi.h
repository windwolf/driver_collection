#ifndef ___BSP_SPI_H__
#define ___BSP_SPI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "common/device.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_spi.h"
#include "stm32h7xx_hal_spi_ex.h"
#include "stm32h7xx_ll_spi.h"
#include "stm32h7xx_ll_dma.h"
    DEVICE_STATUS spi_device_create(SpiDevice *device, SPI_HandleTypeDef *instance, uint16_t dmaThershold);

#ifdef __cplusplus
}
#endif

#endif // ___BSP_SPI_H__