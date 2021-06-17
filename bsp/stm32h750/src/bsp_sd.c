#include "../inc/bsp_sd.h"

SdDevice *_device;

static void _sd_tx_cplt_cb(SD_HandleTypeDef *handle)
{
    if (_device->onTxComplete)
    {
        _device->onTxComplete(_device);
    }
};

static void _sd_rx_cplt_cb(SD_HandleTypeDef *handle)
{
    SCB_InvalidateDCache_by_Addr(_device->_rxBuffer.data, _device->_rxBuffer.size);

    if (_device->onRxComplete)
    {
        _device->onRxComplete(_device);
    }
}

static void _sd_err_cb(SD_HandleTypeDef *handle)
{

    SCB_InvalidateDCache_by_Addr(_device->_rxBuffer.data, _device->_rxBuffer.size);

    if (_device->base.onError)
    {
        _device->base.onError((DeviceBase *)_device, handle->ErrorCode);
    }
};

DEVICE_STATUS sd_device_create(SdDevice *device, SD_HandleTypeDef *instance, uint16_t dmaThershold)
{
    device_base_create((DeviceBase *)device);
    device->base.instance = instance;
    device->_rxBuffer.data = 0;
    device->_rxBuffer.size = 0;
    device->dmaThershold = dmaThershold;
    device->onTxComplete = NULL;
    device->onRxComplete = NULL;
    HAL_SD_RegisterCallback(instance, HAL_SD_RX_CPLT_CB_ID, &_sd_rx_cplt_cb);
    HAL_SD_RegisterCallback(instance, HAL_SD_TX_CPLT_CB_ID, &_sd_tx_cplt_cb);
    HAL_SD_RegisterCallback(instance, HAL_SD_ERROR_CB_ID, &_sd_err_cb);
    _device = device;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS sd_device_init(SdDevice *device)
{
    SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);
    HAL_SD_InitCard(handle);
    return DEVICE_STATUS_OK;
};
DEVICE_STATUS sd_device_deinit(SdDevice *device)
{
    //SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);
    return DEVICE_STATUS_OK;
};
DEVICE_STATUS sd_device_block_read(SdDevice *device, void *data, uint32_t num, uint32_t count)
{
    SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);

    device->_rxBuffer.data = data;
    device->_rxBuffer.size = device->blockSize * count;
    return HAL_SD_WriteBlocks_DMA(handle, (uint8_t *)data, num, count);
};

DEVICE_STATUS sd_device_block_write(SdDevice *device, void *data, uint32_t num, uint32_t count)
{
    SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);

    SCB_CleanDCache_by_Addr((uint32_t *)data, device->blockSize * count);
    return HAL_SD_ReadBlocks_DMA(handle, data, num, count);
};

DEVICE_STATUS sd_device_block_erase(SdDevice *device, uint32_t num, uint32_t count)
{
    SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);
    return HAL_SD_Erase(handle, num, num + count);
};

DEVICE_STATUS sd_device_query_status(SdDevice *device)
{
    SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);
    return ((HAL_SD_GetCardState(handle) == HAL_SD_CARD_TRANSFER) ? DEVICE_STATUS_OK : DEVICE_STATUS_BUSY);
}

DEVICE_STATUS sd_device_block_create(SdDevice *device, Block *block, Buffer buffer)
{
    block_create(block, device,
                 device->blockSize, device->blockSize, device->blockSize,
                 false,
                 BLOCK_MODE_BLOCK, BLOCK_MODE_BLOCK, BLOCK_MODE_BLOCK,
                 buffer,
                 &sd_device_block_read, &sd_device_block_write, &sd_device_block_erase);
    return DEVICE_STATUS_OK;
}

DEVICE_STATUS sd_device_card_init(SdDevice *device)
{
    SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);
    HAL_SD_CardInfoTypeDef cardInfo;
    HAL_SD_GetCardInfo(handle, &cardInfo);
    device->blockSize = cardInfo.BlockSize;
    device->blockNbr = cardInfo.BlockNbr;
    device->cardSpeed = cardInfo.CardSpeed;
    device->cardType = cardInfo.CardType;
    device->cardVersion = cardInfo.CardVersion;
    device->class = cardInfo.Class;
    device->logBlockSize = cardInfo.LogBlockSize;
    device->logBlockNbr = cardInfo.LogBlockNbr;
    device->relCardAdd = cardInfo.RelCardAdd;
    return DEVICE_STATUS_OK;
}