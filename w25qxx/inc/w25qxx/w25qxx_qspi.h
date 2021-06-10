#ifndef ___W25QXX_QSPI_H__
#define ___W25QXX_QSPI_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "w25qxx_base.h"
#include "../../../basic/inc/basic/five_step_command_client.h"
#include "tx_api.h"
#include "stdint.h"
#include "../../../basic/inc/basic/device.h"
    typedef struct W25QXX_QSPI
    {

    } W25QXX_QSPI;

    DEVICE_STATUS w25qxx_qspi_create(W25QXX_QSPI *instance, Buffer buffer, FiveStepCommandCient *command);
    DEVICE_STATUS w25qxx_qspi_reset(W25QXX_QSPI *instance);
    DEVICE_STATUS w25qxx_qspi_status_get(W25QXX_QSPI *instance);
    DEVICE_STATUS w25qxx_qspi_status_set(W25QXX_QSPI *instance);
    DEVICE_STATUS w25qxx_qspi_is_busy(W25QXX_QSPI *instance);
    DEVICE_STATUS w25qxx_qspi_write_enable(W25QXX_QSPI *instance);
    DEVICE_STATUS w25qxx_qspi_id_read(W25QXX_QSPI *instance, uint32_t *id);
    DEVICE_STATUS w25qxx_qspi_read(W25QXX_QSPI *instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
    DEVICE_STATUS w25qxx_qspi_write(W25QXX_QSPI *instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
    DEVICE_STATUS w25qxx_qspi_block_erase(W25QXX_QSPI *instance, uint32_t Address);
    DEVICE_STATUS w25qxx_qspi_chip_erase(W25QXX_QSPI *instance);

#ifdef __cplusplus
}
#endif

#endif // ___W25QXX_QSPI_H__