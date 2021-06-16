#include "../inc/bsp_spi.h"
#include "../inc/bsp_shared.h"

typedef enum SPI_INDEX
{
    SPI1_INDEX = 0,
    SPI2_INDEX,
    SPI3_INDEX,
    SPI4_INDEX,
    SPI6_INDEX,
    SPI_COUNT,
} SPI_INDEX;

DEFINE_DEVICE_REGISTER_BEGIN(SPI, SPI_COUNT)
DEFINE_DEVICE_REGISTER_ITEM(SPI1_BASE, SPI1_INDEX)
DEFINE_DEVICE_REGISTER_ITEM(SPI2_BASE, SPI2_INDEX)
DEFINE_DEVICE_REGISTER_ITEM(SPI3_BASE, SPI3_INDEX)
DEFINE_DEVICE_REGISTER_ITEM(SPI4_BASE, SPI4_INDEX)
DEFINE_DEVICE_REGISTER_ITEM(SPI6_BASE, SPI6_INDEX)
DEFINE_DEVICE_REGISTER_END(SPI)

static void Spi_TxCpltCallback__(SPI_HandleTypeDef *handle)
{
    SpiDevice *device = DEVICE_INSTANCE_FIND(handle->Instance);
    if (device->onTxComplete)
    {
        device->onTxComplete(device);
    }
};

static void Spi_RxCpltCallback__(SPI_HandleTypeDef *handle)
{
    SpiDevice *device = DEVICE_INSTANCE_FIND(handle->Instance);
    if (device->_status.isDmaRx)
    {
        SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
    }

    if (device->onRxComplete)
    {
        device->onRxComplete(device);
    }
}

static void Spi_ErrCallback__(SPI_HandleTypeDef *handle)
{
    SpiDevice *device = DEVICE_INSTANCE_FIND(handle->Instance);
    if (device->_status.isDmaRx)
    {
        SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
    }

    if (device->base.onError)
    {
        device->base.onError((DeviceBase *)device, handle->ErrorCode);
    }
};

static uint32_t bits_switch(SPI_HandleTypeDef *handle, DeviceDataWidth width, uint32_t size)
{

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

DEVICE_STATUS spi_device_create(SpiDevice *device, SPI_HandleTypeDef *instance, uint16_t dmaThershold)
{
    device_base_create((DeviceBase *)device);
    device->base.instance = instance;
    device->_rxBuffer.data = 0;
    device->_rxBuffer.size = 0;
    device->dmaThershold = dmaThershold;
    device->onTxComplete = NULL;
    device->onRxComplete = NULL;
    HAL_SPI_RegisterCallback(instance, HAL_SPI_TX_COMPLETE_CB_ID, &Spi_TxCpltCallback__);
    HAL_SPI_RegisterCallback(instance, HAL_SPI_RX_COMPLETE_CB_ID, &Spi_RxCpltCallback__);
    HAL_SPI_RegisterCallback(instance, HAL_SPI_ERROR_CB_ID, &Spi_ErrCallback__);
    DEVICE_INSTANCE_REGISTER(device, instance->Instance);
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS spi_device_init(SpiDevice *device) { return DEVICE_STATUS_OK; };
DEVICE_STATUS spi_device_deinit(SpiDevice *device) { return DEVICE_STATUS_OK; };

DEVICE_STATUS spi_device_tx(SpiDevice *device, void *data, uint32_t size, DeviceDataWidth width)
{
    SPI_HandleTypeDef *handle = (SPI_HandleTypeDef *)(device->base.instance);
    uint32_t byteSize = bits_switch(handle, width, size);
    if (size < 0)
    {
        return DEVICE_STATUS_GENERAL_ERROR;
    }
    if (size > device->dmaThershold)
    {
        device->_status.isDmaTx = 1;
        SCB_CleanDCache_by_Addr((uint32_t *)data, byteSize);
        return HAL_SPI_Transmit_DMA(handle, data, size);
    }
    else
    {
        device->_status.isDmaTx = 0;
        return HAL_SPI_Transmit_IT(handle, data, size);
    }
};

DEVICE_STATUS spi_device_rx(SpiDevice *device, void *data, uint32_t size, DeviceDataWidth width, uint8_t dummyCycleCount)
{
    SPI_HandleTypeDef *handle = (SPI_HandleTypeDef *)(device->base.instance);
    uint32_t byteSize = bits_switch(handle, width, size);
    if (size < 0)
    {
        return DEVICE_STATUS_GENERAL_ERROR;
    }
    device->_rxBuffer.data = data;
    device->_rxBuffer.size = byteSize;
    if (size > device->dmaThershold)
    {
        device->_status.isDmaRx = 1;
        return HAL_SPI_Receive_DMA(handle, (uint8_t *)data, size);
    }
    else
    {
        device->_status.isDmaRx = 0;
        return HAL_SPI_Receive_IT(handle, (uint8_t *)data, size);
    }
};
