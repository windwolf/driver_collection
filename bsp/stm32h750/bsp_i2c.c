#include "../inc/bsp_i2c.h"

#include "../inc/bsp_shared.h"

#ifdef HAL_I2C_MODULE_ENABLED

typedef enum I2C_INDEX {
    I2C1_INDEX = 0,
    I2C2_INDEX,
    I2C3_INDEX,
    I2C4_INDEX,
    I2C_COUNT,
} I2C_INDEX;

DEFINE_DEVICE_REGISTER_BEGIN(I2C, I2C_COUNT)
DEFINE_DEVICE_REGISTER_ITEM(I2C1_BASE, I2C1_INDEX)
DEFINE_DEVICE_REGISTER_ITEM(I2C2_BASE, I2C2_INDEX)
DEFINE_DEVICE_REGISTER_ITEM(I2C3_BASE, I2C3_INDEX)
DEFINE_DEVICE_REGISTER_ITEM(I2C4_BASE, I2C4_INDEX)
DEFINE_DEVICE_REGISTER_END(I2C)

static void I2C_TxCpltCallback__(I2C_HandleTypeDef *handle) {
    I2CDevice *device = DEVICE_INSTANCE_FIND(handle->Instance);
    if (device->onWriteComplete) {
        device->onWriteComplete(device);
    }
};

static void I2C_RxCpltCallback__(I2C_HandleTypeDef *handle) {
    I2CDevice *device = DEVICE_INSTANCE_FIND(handle->Instance);
    if (device->_status.isDmaRx) {
        SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
    }
    if (device->onReadComplete) {
        device->onReadComplete(device);
    }
}

static void I2C_ErrCallback__(I2C_HandleTypeDef *handle) {
    I2CDevice *device = DEVICE_INSTANCE_FIND(handle->Instance);
    if (device->_status.isDmaRx) {
        SCB_InvalidateDCache_by_Addr(device->_rxBuffer.data, device->_rxBuffer.size);
    }
    if (device->base.onError) {
        device->base.onError((DeviceBase *)device, handle->ErrorCode);
    }
};

OP_RESULT i2c_device_create(I2CDevice *device, I2C_HandleTypeDef *instance, uint16_t dmaThershold) {
    device_base_create((DeviceBase *)device);
    device->base.instance  = instance;
    device->_rxBuffer.data = 0;
    device->_rxBuffer.size = 0;
    device->dmaThershold   = dmaThershold;
    HAL_I2C_RegisterCallback(instance, HAL_I2C_MEM_TX_COMPLETE_CB_ID, &I2C_TxCpltCallback__);
    HAL_I2C_RegisterCallback(instance, HAL_I2C_MEM_RX_COMPLETE_CB_ID, &I2C_RxCpltCallback__);
    HAL_I2C_RegisterCallback(instance, HAL_I2C_ERROR_CB_ID, &I2C_ErrCallback__);
    device->onReadComplete  = NULL;
    device->onWriteComplete = NULL;
    DEVICE_INSTANCE_REGISTER(device, instance->Instance);
    return OP_RESULT_OK;
};

OP_RESULT i2c_device_init(I2CDevice *device) {
    return OP_RESULT_OK;
};
OP_RESULT i2c_device_deinit(I2CDevice *device) {
    return OP_RESULT_OK;
};

OP_RESULT i2c_device_read(I2CDevice *device, uint16_t deviceAddress, void *data, uint32_t size,
                          DeviceDataWidth width) {
    if (width > DEVICE_DATAWIDTH_16) {
        return OP_RESULT_NOT_SUPPORT;
    }
    device->_rxBuffer.data = data;
    device->_rxBuffer.size = size * (width - 1);
    if (device->options.useRxDma && (size > device->options.dmaThershold)) {
        device->_status.isDmaRx = 1;
        return HAL_I2C_Read_DMA(
            (I2C_HandleTypeDef *)device->base.instance, deviceAddress,
            width == DEVICE_DATAWIDTH_8 ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT,
            (uint8_t *)data, size /*TODO: 确认要不要x2 */);
    } else {
        device->_status.isDmaRx = 0;
        return HAL_I2C_Read_DMA(
            (I2C_HandleTypeDef *)device->base.instance, deviceAddress,
            width == DEVICE_DATAWIDTH_8 ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT,
            (uint8_t *)data, size /*TODO: 确认要不要x2 */);
    }
};
OP_RESULT i2c_device_write(I2CDevice *device, uint16_t deviceAddress, void *data, uint32_t size,
                           DeviceDataWidth width) {
    if (width > DEVICE_DATAWIDTH_16) {
        return OP_RESULT_NOT_SUPPORT;
    }
    if (device->options.useTxDma && (size > device->options.dmaThershold)) {
        device->_status.isDmaTx = 1;
        SCB_CleanDCache_by_Addr((uint32_t *)data, size * (width - 1));
        return HAL_I2C_Write_DMA(
            (I2C_HandleTypeDef *)device->base.instance, deviceAddress,
            width == DEVICE_DATAWIDTH_8 ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT,
            (uint8_t *)data, size /*TODO: 确认要不要x2 */);
    } else {
        device->_status.isDmaTx = 0;
        return HAL_I2C_Write_IT(
            (I2C_HandleTypeDef *)device->base.instance, deviceAddress,
            width == DEVICE_DATAWIDTH_8 ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT,
            (uint8_t *)data, size /*TODO: 确认要不要x2 */);
    }
};

OP_RESULT i2c_device_mem_write(I2CDevice *device, uint16_t deviceAddress, uint16_t memAddress,
                               void *data, uint32_t size, DeviceDataWidth width) {
    if (width > DEVICE_DATAWIDTH_16) {
        return OP_RESULT_NOT_SUPPORT;
    }
    if (device->options.useTxDma && (size > device->options.dmaThershold)) {
        device->_status.isDmaTx = 1;
        SCB_CleanDCache_by_Addr((uint32_t *)data, size * (width - 1));
        return HAL_I2C_Mem_Write_DMA(
            (I2C_HandleTypeDef *)device->base.instance, deviceAddress, memAddress,
            width == DEVICE_DATAWIDTH_8 ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT,
            (uint8_t *)data, size /*TODO: 确认要不要x2 */);
    } else {
        device->_status.isDmaTx = 0;
        return HAL_I2C_Mem_Write_IT(
            (I2C_HandleTypeDef *)device->base.instance, deviceAddress, memAddress,
            width == DEVICE_DATAWIDTH_8 ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT,
            (uint8_t *)data, size /*TODO: 确认要不要x2 */);
    }
};

OP_RESULT i2c_device_mem_read(I2CDevice *device, uint16_t deviceAddress, uint16_t memAddress,
                              void *data, uint32_t size, DeviceDataWidth width) {
    if (width > DEVICE_DATAWIDTH_16) {
        return OP_RESULT_NOT_SUPPORT;
    }
    device->_rxBuffer.data = data;
    device->_rxBuffer.size = size * (width - 1);
    if (device->options.useRxDma && (size > device->options.dmaThershold)) {
        device->_status.isDmaRx = 1;
        return HAL_I2C_Mem_Read_DMA(
            (I2C_HandleTypeDef *)device->base.instance, deviceAddress, memAddress,
            width == DEVICE_DATAWIDTH_8 ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT,
            (uint8_t *)data, size /*TODO: 确认要不要x2 */);
    } else {
        device->_status.isDmaRx = 0;
        return HAL_I2C_Mem_Read_IT(
            (I2C_HandleTypeDef *)device->base.instance, deviceAddress, memAddress,
            width == DEVICE_DATAWIDTH_8 ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT,
            (uint8_t *)data, size /*TODO: 确认要不要x2 */);
    }
};

#endif  // HAL_I2C_MODULE_ENABLED
