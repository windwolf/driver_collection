#include "bsp/uart_device.hpp"

using namespace Windwolf::Drivers::Bsp;
using namespace Windwolf::Drivers;

void STM32H7xxUartDeviceHandle_Init(STM32H7xxUartDeviceHandle *handle, void *device) {
    handle->device = device;
}

extern "C" void Uart_TxCpltCallback__(UART_HandleTypeDef *huart) {

    STM32H7xxUartDeviceHandle *handle = (STM32H7xxUartDeviceHandle *) huart;
    ((STM32H7xxUartDevice *) handle->device)->TxNotify();
}
extern "C" void Uart_RxCpltCallback__(UART_HandleTypeDef *huart) {

    STM32H7xxUartDeviceHandle *handle = (STM32H7xxUartDeviceHandle *) huart;
    ((STM32H7xxUartDevice *) handle->device)->RxNotify();
}

STM32H7xxUartDevice::STM32H7xxUartDevice(STM32H7xxUartDeviceHandle *handle) {
    _handle = (UART_HandleTypeDef *) (handle);
    _handle->TxCpltCallback = Uart_TxCpltCallback__;
    _handle->RxCpltCallback = Uart_RxCpltCallback__;
}


DEVICE_STATUS STM32H7xxUartDevice::TxAsync(uint8_t *writeData, uint32_t dataSize, WaitHandle *txSync) {
    if (_txSync) {
        return DEVICE_STATUS::BUSY;
    }
    _txSync = txSync;
    _txSync->Start();
    if (HAL_UART_Transmit_DMA(_handle, writeData, dataSize) != HAL_OK) {
        _txSync->Error();
        return DEVICE_STATUS::HARDWARE_ERROR;
    }

    return DEVICE_STATUS::OK;
}

DEVICE_STATUS STM32H7xxUartDevice::RxAsync(uint8_t *readBuffer, uint32_t bufferSize, WaitHandle *rxSync) {
    if (_rxSync) {
        return DEVICE_STATUS::BUSY;
    }
    _rxSync = rxSync;
    _rxSync->Start();
    if (HAL_UART_Receive_DMA(_handle, readBuffer, bufferSize) != HAL_OK) {
        _rxSync->Error();
        return DEVICE_STATUS::HARDWARE_ERROR;
    }
    return DEVICE_STATUS::OK;
}

DEVICE_STATUS STM32H7xxUartDevice::TxNotify() {
    if (_txSync) {
        auto sync = _txSync;
        _txSync = nullptr;
        sync->Finish();
        return DEVICE_STATUS::OK;
    }
    return DEVICE_STATUS::GENERAL_ERROR;
};

DEVICE_STATUS STM32H7xxUartDevice::RxNotify() {
    if (_rxSync) {
        //TODO: set payload.
        auto sync = _rxSync;
        _rxSync = nullptr;
        sync->Finish();
        return DEVICE_STATUS::OK;
    }
    return DEVICE_STATUS::GENERAL_ERROR;
};



