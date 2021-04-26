#include "bsp/uart_device.hpp"

using namespace Windwolf::Drivers::Bsp;
using namespace Windwolf::Drivers;

static STM32H7xxUartDevice *INSTANCES__[(uint32_t) (UartIndex::UART_COUNT)];

static inline UartIndex GetInstanceIndex(UART_HandleTypeDef *huart) {
    auto ins = (uint32_t) (huart->Instance);

    switch (ins) {
        case UART4_BASE:
            return UartIndex::UART4_INDEX;
        case UART5_BASE:
            return UartIndex::UART5_INDEX;
        case UART7_BASE:
            return UartIndex::UART7_INDEX;
        case UART8_BASE:
            return UartIndex::UART8_INDEX;
        default:
            return Windwolf::Drivers::Bsp::UartIndex::UART_COUNT;
    }
}

static inline void RegisterDevice(STM32H7xxUartDevice *device, UART_HandleTypeDef *huart) {
    auto index = GetInstanceIndex(huart);
    INSTANCES__[(uint32_t) index] = device;

}

static inline STM32H7xxUartDevice *GetDevice(UART_HandleTypeDef *huart) {
    auto index = GetInstanceIndex(huart);
    return INSTANCES__[(uint32_t) index];
}

//void STM32H7xxUartDeviceHandle_Init(STM32H7xxUartDeviceHandle *handle, void *device) {
//    handle->device = device;
//}

extern "C" void Uart_TxCpltCallback__(UART_HandleTypeDef *huart) {
    auto device = GetDevice(huart);
    device->TxNotify();
}
extern "C" void Uart_RxCpltCallback__(UART_HandleTypeDef *huart) {
    auto device = GetDevice(huart);
    device->RxNotify();
}

STM32H7xxUartDevice::STM32H7xxUartDevice(UART_HandleTypeDef *handle)
        : _handle(handle) {

    _handle->TxCpltCallback = Uart_TxCpltCallback__;
    _handle->RxCpltCallback = Uart_RxCpltCallback__;
    RegisterDevice(this, handle);
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



