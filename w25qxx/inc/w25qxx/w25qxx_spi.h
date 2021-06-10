#ifndef ___W25QXX_SPI_H__
#define ___W25QXX_SPI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "w25qxx_base.h"
#include "../../../basic/inc/basic/five_step_command_client.h"
#include "tx_api.h"
#include "stdint.h"
#include "../../../basic/inc/basic/device.h"

    typedef struct W25QXX_SPI
    {
        W25QXX_BASE base;
        FiveStepCommandCient *command;
    } W25QXX_SPI;

    DEVICE_STATUS w25qxx_spi_create(W25QXX_SPI *instance, Buffer buffer, FiveStepCommandCient *command);
    DEVICE_STATUS w25qxx_spi_reset(W25QXX_SPI *instance);
    DEVICE_STATUS w25qxx_spi_status_get(W25QXX_SPI *instance);
    DEVICE_STATUS w25qxx_spi_status_set(W25QXX_SPI *instance);
    DEVICE_STATUS w25qxx_spi_is_busy(W25QXX_SPI *instance);
    DEVICE_STATUS w25qxx_spi_write_enable(W25QXX_SPI *instance);
    DEVICE_STATUS w25qxx_spi_id_read(W25QXX_SPI *instance, uint32_t *id);
    DEVICE_STATUS w25qxx_spi_read(W25QXX_SPI *instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
    DEVICE_STATUS w25qxx_spi_write(W25QXX_SPI *instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
    DEVICE_STATUS w25qxx_spi_block_erase(W25QXX_SPI *instance, uint32_t Address);
    DEVICE_STATUS w25qxx_spi_chip_erase(W25QXX_SPI *instance);

#ifdef __cplusplus
}
#endif

#endif // ___W25QXX_SPI_H__