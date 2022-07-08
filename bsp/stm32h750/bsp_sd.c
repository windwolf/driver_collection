#include "../inc/bsp_sd.h"
#include "../inc/bsp_shared.h"

typedef enum SD_INDEX
{
    SD1_INDEX = 0,
    SD2_INDEX,
    SD_COUNT,
} SD_INDEX;

DEFINE_DEVICE_REGISTER_BEGIN(SD, SD_COUNT)
DEFINE_DEVICE_REGISTER_ITEM(SDMMC1_BASE, SD1_INDEX)
DEFINE_DEVICE_REGISTER_ITEM(SDMMC2_BASE, SD2_INDEX)
DEFINE_DEVICE_REGISTER_END(SD)

static void _sd_tx_cplt_cb(SD_HandleTypeDef *handle)
{
    SdDevice *device = DEVICE_INSTANCE_FIND(handle->Instance);
    if (device->onTxComplete)
    {
        device->onTxComplete(device);
    }
};

static void _sd_rx_cplt_cb(SD_HandleTypeDef *handle)
{
    SdDevice *device = DEVICE_INSTANCE_FIND(handle->Instance);
    SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);

    if (device->onRxComplete)
    {
        device->onRxComplete(device);
    }
}

static void _sd_err_cb(SD_HandleTypeDef *handle)
{
    SdDevice *device = DEVICE_INSTANCE_FIND(handle->Instance);
    SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);

    if (device->base.onError)
    {
        device->base.onError((DeviceBase *)device, handle->ErrorCode);
    }
};

OP_RESULT sd_device_create(SdDevice *device, SD_HandleTypeDef *instance, uint16_t dmaThershold)
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
    DEVICE_INSTANCE_REGISTER(device, instance->Instance);
    return OP_RESULT_OK;
};

OP_RESULT sd_device_init(SdDevice *device)
{
    SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);
    return HAL_SD_InitCard(handle);
};
OP_RESULT sd_device_deinit(SdDevice *device)
{
    // SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);
    return OP_RESULT_OK;
};
OP_RESULT sd_device_read(SdDevice *device, void *data, uint32_t num, uint32_t count)
{
    SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);

    device->_rxBuffer.data = data;
    device->_rxBuffer.size = device->blockSize * count;
    return HAL_SD_ReadBlocks_DMA(handle, (uint8_t *)data, num, count);
};

OP_RESULT sd_device_write(SdDevice *device, void *data, uint32_t num, uint32_t count)
{
    SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);

    SCB_CleanDCache_by_Addr((uint32_t *)data, device->blockSize * count);
    return HAL_SD_WriteBlocks_DMA(handle, data, num, count);
};

OP_RESULT sd_device_erase(SdDevice *device, uint32_t num, uint32_t count)
{
    SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);
    return HAL_SD_Erase(handle, num, num + count);
};

OP_RESULT sd_device_query_status(SdDevice *device)
{
    SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);
    return ((HAL_SD_GetCardState(handle) == HAL_SD_CARD_TRANSFER) ? OP_RESULT_OK : OP_RESULT_BUSY);
}

OP_RESULT sd_device_block_create(SdDevice *device, Block *block, Buffer buffer)
{
    return block_create(block, device,
                        device->blockSize, device->blockSize, device->blockSize,
                        false,
                        BLOCK_MODE_BLOCK, BLOCK_MODE_BLOCK, BLOCK_MODE_BLOCK,
                        buffer,
                        (BLOCK_READ_WRITE_CB)&sd_device_read, (BLOCK_READ_WRITE_CB)&sd_device_write, (BLOCK_ERROR_CB)&sd_device_erase);
}

OP_RESULT sd_device_card_init(SdDevice *device)
{
    SD_HandleTypeDef *handle = (SD_HandleTypeDef *)(device->base.instance);
    HAL_SD_CardInfoTypeDef cardInfo;
    OP_RESULT rst;
    rst = HAL_SD_InitCard(handle);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = HAL_SD_GetCardInfo(handle, &cardInfo);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    device->blockSize = cardInfo.BlockSize;
    device->blockNbr = cardInfo.BlockNbr;
    device->cardSpeed = cardInfo.CardSpeed;
    device->cardType = cardInfo.CardType;
    device->cardVersion = cardInfo.CardVersion;
    device->class = cardInfo.Class;
    device->logBlockSize = cardInfo.LogBlockSize;
    device->logBlockNbr = cardInfo.LogBlockNbr;
    device->relCardAdd = cardInfo.RelCardAdd;

    return OP_RESULT_OK;
}