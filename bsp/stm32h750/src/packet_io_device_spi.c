#include "packet_io_device_spi.h"

typedef enum SPI_INDEX
{
    SPI1_INDEX = 0,
    SPI2_INDEX,
    SPI3_INDEX,
    SPI4_INDEX,
    SPI6_INDEX,
    SPI_COUNT,
} SPI_INDEX;

static PacketIoDevice *SPI_INSTANCE_[(uint32_t)(SPI_COUNT)];

static inline SPI_INDEX Instance_Index_Get(SPI_HandleTypeDef *handle)
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

static inline void Instance_Register(PacketIoDevice *device)
{
    SPI_INDEX index = Instance_Index_Get((SPI_HandleTypeDef *)(device->instance));
    SPI_INSTANCE_[(uint32_t)index] = device;
}

static inline PacketIoDevice *Instance_Find(SPI_HandleTypeDef *huart)
{
    SPI_INDEX index = Instance_Index_Get(huart);
    return SPI_INSTANCE_[(uint32_t)index];
}

static void Spi_TxCpltCallback__(SPI_HandleTypeDef *handle)
{
    PacketIoDevice *device = Instance_Find(handle);
    if (device->onTxComplete)
    {
        device->onTxComplete(device);
    }
};

static void Spi_RxCpltCallback__(SPI_HandleTypeDef *handle)
{
    PacketIoDevice *device = Instance_Find(handle);
    SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
    if (device->onRxComplete)
    {
        device->onRxComplete(device);
    }
}

static void Spi_ErrCallback__(SPI_HandleTypeDef *handle)
{
    PacketIoDevice *device = Instance_Find(handle);
    SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
    if (device->onError)
    {
        device->onError(device);
    }
};

static void Init(PacketIoDevice *device)
{
}

static uint32_t SwitchBits(PacketIoDevice *device, uint8_t width, uint32_t size)
{
    SPI_HandleTypeDef *handle = (SPI_HandleTypeDef *)(device->instance);
    uint32_t stream_number_tx = (((uint32_t)((uint32_t *)handle->hdmatx->Instance) & 0xFFU) - 0x010UL) / 0x018UL;
    uint32_t dma_base_tx = (uint32_t)((uint32_t *)handle->hdmatx->Instance) - stream_number_tx * 0x018UL - 0x010UL;
    uint32_t stream_number_rx = (((uint32_t)((uint32_t *)handle->hdmarx->Instance) & 0xFFU) - 0x010UL) / 0x018UL;
    uint32_t dma_base_rx = (uint32_t)((uint32_t *)handle->hdmarx->Instance) - stream_number_rx * 0x018UL - 0x010UL;

    switch (width)
    {
    case DEVICE_DATAWIDTH_8:
        handle->Init.DataSize = SPI_DATASIZE_8BIT;
        LL_SPI_SetDataWidth(handle->Instance, LL_SPI_DATAWIDTH_8BIT);
        handle->hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        handle->hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        handle->hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        handle->hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_MDATAALIGN_BYTE);
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_MDATAALIGN_BYTE);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_PDATAALIGN_BYTE);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_PDATAALIGN_BYTE);
        return size;
    case DEVICE_DATAWIDTH_16:
        handle->Init.DataSize = SPI_DATASIZE_16BIT;
        LL_SPI_SetDataWidth(handle->Instance, LL_SPI_DATAWIDTH_16BIT);
        handle->hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        handle->hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        handle->hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        handle->hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_MDATAALIGN_HALFWORD);
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_MDATAALIGN_HALFWORD);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_PDATAALIGN_HALFWORD);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_PDATAALIGN_HALFWORD);
        return size << 1;
    case DEVICE_DATAWIDTH_24:
        handle->Init.DataSize = SPI_DATASIZE_24BIT;
        LL_SPI_SetDataWidth(handle->Instance, LL_SPI_DATAWIDTH_24BIT);
        handle->hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        handle->hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        handle->hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        handle->hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_MDATAALIGN_WORD);
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_MDATAALIGN_WORD);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_PDATAALIGN_WORD);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_PDATAALIGN_WORD);
        return size << 2;
    case DEVICE_DATAWIDTH_32:
        handle->Init.DataSize = SPI_DATASIZE_32BIT;
        LL_SPI_SetDataWidth(handle->Instance, LL_SPI_DATAWIDTH_32BIT);
        handle->hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        handle->hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        handle->hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        handle->hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_MDATAALIGN_WORD);
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_MDATAALIGN_WORD);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_PDATAALIGN_WORD);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_PDATAALIGN_WORD);
        return size << 2;
    default:
        return 0;
    }
    //return;
};

static DEVICE_STATUS Tx(PacketIoDevice *device, void *data, uint32_t size, uint8_t width)
{
    uint32_t byteSize = SwitchBits(device, width, size);
    if (size < 0)
    {
        return DEVICE_STATUS_ARGUMENT_ERROR;
    }

    if (HAL_SPI_Transmit(device->instance, data, size, HAL_MAX_DELAY) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

static DEVICE_STATUS Rx(PacketIoDevice *device, void *buffer, uint32_t size, uint8_t width, uint8_t dummyCycleCount)
{
    uint32_t byteSize = SwitchBits(device, width, size);
    if (size < 0)
    {
        return DEVICE_STATUS_ARGUMENT_ERROR;
    }
    if (HAL_SPI_Receive(device->instance, buffer, size, HAL_MAX_DELAY) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

static DEVICE_STATUS TxAsync(PacketIoDevice *device, uint8_t *data, uint32_t size, uint8_t width)
{
    uint32_t byteSize = SwitchBits(device, width, size);
    if (size < 0)
    {
        return DEVICE_STATUS_ARGUMENT_ERROR;
    }
    SCB_CleanDCache_by_Addr((uint32_t *)data, byteSize);
    if (HAL_SPI_Transmit_DMA(device->instance, data, size) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

static DEVICE_STATUS RxAsync(PacketIoDevice *device, uint8_t *buffer, uint32_t size, uint8_t width, uint8_t dummyCycleCount)
{
    uint32_t byteSize = SwitchBits(device, width, size);
    if (size < 0)
    {
        return DEVICE_STATUS_ARGUMENT_ERROR;
    }
    device->_rxBuffer.data = buffer;
    device->_rxBuffer.size = byteSize;
    if (HAL_SPI_Receive_DMA(device->instance, (uint8_t *)buffer, size) == HAL_OK)
    {
        return DEVICE_STATUS_OK;
    }
    return DEVICE_STATUS_HARDWARE_ERROR;
};

void spi_packet_io_device_create(PacketIoDevice *device, SPI_HandleTypeDef *handle)
{
    device->instance = handle;
    device->Init = &Init;
    device->Tx = &Tx;
    device->TxAsync = &TxAsync;
    device->Rx = &Rx;
    device->RxAsync = &RxAsync;
    device->opMode = PACKET_IO_DEVICE_OP_MODE_SYNC | PACKET_IO_DEVICE_OP_MODE_ASYNC;

    device->_rxBuffer.data = 0;
    device->_rxBuffer.size = 0;

    Instance_Register(device);
    HAL_SPI_RegisterCallback(handle, HAL_SPI_TX_COMPLETE_CB_ID, Spi_TxCpltCallback__);
    HAL_SPI_RegisterCallback(handle, HAL_SPI_RX_COMPLETE_CB_ID, Spi_RxCpltCallback__);
    HAL_SPI_RegisterCallback(handle, HAL_SPI_ERROR_CB_ID, Spi_ErrCallback__);
};
