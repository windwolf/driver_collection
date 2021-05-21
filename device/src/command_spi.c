#include "device/command_spi.h"

typedef enum SPI_INDEX
{
    SPI1_INDEX = 0,
    SPI2_INDEX,
    SPI3_INDEX,
    SPI4_INDEX,
    SPI6_INDEX,
    SPI_COUNT,
} SPI_INDEX;

static CommandMaster *SPI_STREAM_INSTANCES[(uint32_t)(SPI_COUNT)];

static inline SPI_INDEX CommandMaster_instance_index_get(SPI_HandleTypeDef *handle)
{
    uint32_t ins = (uint32_t)(handle->Instance);

    switch (ins)
    {
    case SPI1_BASE:
        return SPI1_INDEX;
    case SPI2_BASE:
        return SPI2_INDEX;
    case SPI3_BASE:
        return SPI3_INDEX;
    case SPI4_BASE:
        return SPI4_INDEX;
    case SPI6_BASE:
        return SPI6_INDEX;
    default:
        return SPI_COUNT;
    }
}

static inline void CommandMaster_register(CommandMasterDevice *commandMaster)
{
    SPI_INDEX index = CommandMaster_instance_index_get((SPI_HandleTypeDef *)(commandMaster->instance));
    SPI_STREAM_INSTANCES[(uint32_t)index] = (CommandMaster *)commandMaster;
}

static inline CommandMaster *CommandMaster_find(SPI_HandleTypeDef *huart)
{
    SPI_INDEX index = CommandMaster_instance_index_get(huart);
    return SPI_STREAM_INSTANCES[(uint32_t)index];
}

static void Spi_TxCpltCallback__(SPI_HandleTypeDef *handle)
{
    CommandMaster *cmdMst = CommandMaster_find(handle);
    CommandMaster_DoTxComplete_(cmdMst);
}

static void Spi_RxCpltCallback__(SPI_HandleTypeDef *handle)
{
    CommandMaster *cmdMst = CommandMaster_find(handle);
    CommandMaster_DoRxComplete_(cmdMst);
}

static void Spi_ErrCallback__(SPI_HandleTypeDef *handle)
{
    CommandMaster *cmdMst = CommandMaster_find(handle);
    CommandMaster_DoError_(cmdMst);
}

static void Init(CommandMasterDevice *device)
{
    CommandMaster_register(device);
    HAL_SPI_RegisterCallback(device->instance, HAL_SPI_TX_COMPLETE_CB_ID, Spi_TxCpltCallback__);
    HAL_SPI_RegisterCallback(device->instance, HAL_SPI_RX_COMPLETE_CB_ID, Spi_RxCpltCallback__);
    HAL_SPI_RegisterCallback(device->instance, HAL_SPI_ERROR_CB_ID, Spi_ErrCallback__);
}

static DEVICE_STATUS TxN(CommandMasterDevice *device, uint8_t *data, uint32_t size)
{
    if (HAL_SPI_Transmit(device->instance, data, size, HAL_MAX_DELAY) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};
static DEVICE_STATUS RxN(struct CommandMasterDevice *device, uint8_t *buffer, uint32_t size, uint8_t dummyCycleCount)
{
    if (HAL_SPI_Receive(device->instance, buffer, size, HAL_MAX_DELAY) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};
static DEVICE_STATUS TxNAsync(struct CommandMasterDevice *device, uint8_t *data, uint32_t size)
{
    if (HAL_SPI_Transmit_DMA(device->instance, data, size) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};
static DEVICE_STATUS RxNAsync(struct CommandMasterDevice *device, uint8_t *buffer, uint32_t size, uint8_t dummyCycleCount)
{
    if (HAL_SPI_Receive_DMA(device->instance, buffer, size) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

void Spi_CommandDevice_Create(CommandMasterDevice *device, SPI_HandleTypeDef *handle)
{
    device->instance = handle;
    device->Init = &Init;
    device->TxN = &TxN;
    device->TxNAsync = &TxNAsync;
    device->RxN = &RxN;
    device->RxNAsync = &RxNAsync;
    device->opMode = COMMAND_DEVICE_OP_MODE_SYNC | COMMAND_DEVICE_OP_MODE_ASYNC;
};
