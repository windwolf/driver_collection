#include "bsp_shared.h"
#include "bsp_spi.h"

#ifdef HAL_SPI_MODULE_ENABLED

typedef enum SPI_INDEX
{
    SPI1_INDEX = 0,
    SPI2_INDEX,
    SPI_COUNT,
} SPI_INDEX;

DEFINE_DEVICE_REGISTER_BEGIN(SPI, SPI_COUNT)
DEFINE_DEVICE_REGISTER_ITEM(SPI1_BASE, SPI1_INDEX)
DEFINE_DEVICE_REGISTER_ITEM(SPI2_BASE, SPI2_INDEX)
DEFINE_DEVICE_REGISTER_END(SPI)

typedef struct SizeInfo
{
    uint32_t sizeInBytes;
    uint32_t sizeInDMADataWidth;
    uint32_t sizeInSPIDataWidth;
} SizeInfo;

static void
Spi_TxCpltCallback__(SPI_HandleTypeDef *handle)
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
        // SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
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
        // SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
    }

    if (device->base.onError)
    {
        device->base.onError((DeviceBase *)device, handle->ErrorCode);
    }
};

static void dma_datawidth_set(SPI_HandleTypeDef *handle, uint32_t stream_number_tx, uint32_t dma_base_tx, uint32_t stream_number_rx, uint32_t dma_base_rx, uint8_t b8)
{
    LL_SPI_SetDataWidth(handle->Instance, (b8 ? LL_SPI_DATAWIDTH_8BIT : LL_SPI_DATAWIDTH_16BIT));
    handle->hdmatx->Init.MemDataAlignment = (b8 ? DMA_MDATAALIGN_BYTE : DMA_MDATAALIGN_HALFWORD);
    handle->hdmarx->Init.MemDataAlignment = (b8 ? DMA_MDATAALIGN_BYTE : DMA_MDATAALIGN_HALFWORD);
    handle->hdmatx->Init.PeriphDataAlignment = (b8 ? DMA_PDATAALIGN_BYTE : DMA_MDATAALIGN_HALFWORD);
    handle->hdmarx->Init.PeriphDataAlignment = (b8 ? DMA_PDATAALIGN_BYTE : DMA_MDATAALIGN_HALFWORD);
    LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_tx, stream_number_tx, (b8 ? LL_DMA_MDATAALIGN_BYTE : LL_DMA_MDATAALIGN_HALFWORD));
    LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_rx, stream_number_rx, (b8 ? LL_DMA_MDATAALIGN_BYTE : LL_DMA_MDATAALIGN_HALFWORD));
    LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_tx, stream_number_tx, (b8 ? LL_DMA_PDATAALIGN_BYTE : LL_DMA_MDATAALIGN_HALFWORD));
    LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_rx, stream_number_rx, (b8 ? LL_DMA_PDATAALIGN_BYTE : LL_DMA_MDATAALIGN_HALFWORD));
}

static void bits_switch(SPI_HandleTypeDef *handle, DeviceDataWidth width, uint32_t size, SizeInfo *sizeInfo)
{

    uint32_t stream_number_tx = (((uint32_t)((uint32_t *)handle->hdmatx->Instance) & 0xFFU) - 0x08UL) / 0x014UL;
    uint32_t dma_base_tx = (uint32_t)((uint32_t *)handle->hdmatx->Instance) - stream_number_tx * 0x014UL - 0x08UL;
    uint32_t stream_number_rx = (((uint32_t)((uint32_t *)handle->hdmarx->Instance) & 0xFFU) - 0x08UL) / 0x014UL;
    uint32_t dma_base_rx = (uint32_t)((uint32_t *)handle->hdmarx->Instance) - stream_number_rx * 0x014UL - 0x08UL;
    switch (width)
    {
    case DEVICE_DATAWIDTH_8:
        handle->Init.DataSize = SPI_DATASIZE_8BIT;
        dma_datawidth_set(handle, stream_number_tx, dma_base_tx, stream_number_rx, dma_base_rx, 1);
        sizeInfo->sizeInBytes = size;
        sizeInfo->sizeInDMADataWidth = size;
        sizeInfo->sizeInSPIDataWidth = size;
        break;
    case DEVICE_DATAWIDTH_16:
        handle->Init.DataSize = SPI_DATASIZE_16BIT;
        dma_datawidth_set(handle, stream_number_tx, dma_base_tx, stream_number_rx, dma_base_rx, 0);
        sizeInfo->sizeInBytes = size << 1;
        sizeInfo->sizeInDMADataWidth = size;
        sizeInfo->sizeInSPIDataWidth = size;
        break;
    case DEVICE_DATAWIDTH_24:
        handle->Init.DataSize = SPI_DATASIZE_8BIT;
        dma_datawidth_set(handle, stream_number_tx, dma_base_tx, stream_number_rx, dma_base_rx, 1);
        sizeInfo->sizeInBytes = size * 3;
        sizeInfo->sizeInDMADataWidth = size * 3;
        sizeInfo->sizeInSPIDataWidth = size * 3;
        break;
    case DEVICE_DATAWIDTH_32:
        handle->Init.DataSize = SPI_DATASIZE_16BIT;
        dma_datawidth_set(handle, stream_number_tx, dma_base_tx, stream_number_rx, dma_base_rx, 0);
        sizeInfo->sizeInBytes = size << 2;
        sizeInfo->sizeInDMADataWidth = size << 2;
        sizeInfo->sizeInSPIDataWidth = size << 2;
        break;
    default:
        break;
    }
    // return;
};

OP_RESULT spi_device_create(SpiDevice *device, SPI_HandleTypeDef *instance, uint16_t dmaThershold)
{
    device_base_create((DeviceBase *)device);
    device->base.instance = instance;
    device->_rxBuffer.data = 0;
    device->_rxBuffer.size = 0;
    device->options.dmaThershold = dmaThershold;
    device->onTxComplete = NULL;
    device->onRxComplete = NULL;
    HAL_SPI_RegisterCallback(instance, HAL_SPI_TX_COMPLETE_CB_ID, &Spi_TxCpltCallback__);
    HAL_SPI_RegisterCallback(instance, HAL_SPI_RX_COMPLETE_CB_ID, &Spi_RxCpltCallback__);
    HAL_SPI_RegisterCallback(instance, HAL_SPI_ERROR_CB_ID, &Spi_ErrCallback__);
    DEVICE_INSTANCE_REGISTER(device, instance->Instance);
    return OP_RESULT_OK;
};

OP_RESULT spi_device_init(SpiDevice *device) { return OP_RESULT_OK; };
OP_RESULT spi_device_deinit(SpiDevice *device) { return OP_RESULT_OK; };

OP_RESULT spi_device_tx(SpiDevice *device, void *data, uint32_t size, DeviceDataWidth width)
{
    SPI_HandleTypeDef *handle = (SPI_HandleTypeDef *)(device->base.instance);
    SizeInfo sizeInfo;
    bits_switch(handle, width, size, &sizeInfo);
    if (size < 0)
    {
        return OP_RESULT_GENERAL_ERROR;
    }
    if (device->options.useTxDma && (size > device->options.dmaThershold))
    {
        device->_status.isDmaTx = 1;
        // SCB_CleanDCache_by_Addr((uint32_t *)data, byteSize);
        return HAL_SPI_Transmit_DMA(handle, data, sizeInfo.sizeInDMADataWidth);
    }
    else
    {
        device->_status.isDmaTx = 0;
        return HAL_SPI_Transmit_IT(handle, data, sizeInfo.sizeInSPIDataWidth);
    }
};

OP_RESULT spi_device_rx(SpiDevice *device, void *data, uint32_t size, DeviceDataWidth width, uint8_t dummyCycleCount)
{
    SPI_HandleTypeDef *handle = (SPI_HandleTypeDef *)(device->base.instance);
    SizeInfo sizeInfo;
    bits_switch(handle, width, size, &sizeInfo);
    if (size < 0)
    {
        return OP_RESULT_GENERAL_ERROR;
    }
    device->_rxBuffer.data = data;
    device->_rxBuffer.size = sizeInfo.sizeInBytes;
    if (device->options.useRxDma && (size > device->options.dmaThershold))
    {
        device->_status.isDmaRx = 1;
        return HAL_SPI_Receive_DMA(handle, (uint8_t *)data, sizeInfo.sizeInDMADataWidth);
    }
    else
    {
        device->_status.isDmaRx = 0;
        return HAL_SPI_Receive_IT(handle, (uint8_t *)data, sizeInfo.sizeInSPIDataWidth);
    }
};

#endif // HAL_SPI_MODULE_ENABLED