#include "bsp/uart_device.hpp"
#include "stm32h7xx_ll_usart.h"

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

extern "C" void Uart_TxCpltCallback__(UART_HandleTypeDef *huart) {
    auto device = GetDevice(huart);
    device->TxNotify();
}

//extern "C" void Uart_RxCpltCallback__(UART_HandleTypeDef *huart) {
//    auto device = GetDevice(huart);
//    device->RxNotify();
//}

extern "C" void Uart_RxEventCpltCallback__(UART_HandleTypeDef *huart, uint16_t pos) {
    auto device = GetDevice(huart);
    device->RxNotify(pos);
}

extern "C" void Uart_ErrCpltCallback__(UART_HandleTypeDef *huart) {
    auto device = GetDevice(huart);
    device->ErrorNotify();
}

STM32H7xxUartDevice::STM32H7xxUartDevice(UART_HandleTypeDef *handle)
        : _handle(handle) {
    HAL_UART_RegisterCallback(_handle, HAL_UART_TX_COMPLETE_CB_ID, Uart_TxCpltCallback__);
    //HAL_UART_RegisterCallback(_handle, HAL_UART_RX_COMPLETE_CB_ID, Uart_RxCpltCallback__);
    HAL_UART_RegisterCallback(_handle, HAL_UART_ERROR_CB_ID, Uart_ErrCpltCallback__);
    HAL_UART_RegisterRxEventCallback(_handle, Uart_RxEventCpltCallback__);
    RegisterDevice(this, handle);
}


DEVICE_STATUS STM32H7xxUartDevice::TxAsync(uint8_t *writeData, uint32_t dataSize, WaitHandle *callback) {

    if ((HAL_UART_GetState(_handle) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX) {
        return DEVICE_STATUS::BUSY;
    }
    _txCallback = callback;
    if (HAL_UART_Transmit_DMA(_handle, writeData, dataSize) != HAL_OK) {
        callback->Error();
        return DEVICE_STATUS::HARDWARE_ERROR;
    }

    return DEVICE_STATUS::OK;
}

DEVICE_STATUS STM32H7xxUartDevice::RxAsync(uint8_t *readBuffer, uint32_t bufferSize, WaitHandle *callback) {
    if (_handle->hdmarx->Init.Mode != DMA_NORMAL) {
        return DEVICE_STATUS::NOT_SUPPORT;
    }
    if ((HAL_UART_GetState(_handle) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX) {
        return DEVICE_STATUS::BUSY;
    }
    _rxCallback = callback;
    //TODO: enable it DMA_RXs
    if (HAL_UARTEx_ReceiveToIdle_DMA(_handle, readBuffer, bufferSize) != HAL_OK) {
        callback->Error();
        return DEVICE_STATUS::HARDWARE_ERROR;
    }
    return DEVICE_STATUS::OK;
}

DEVICE_STATUS STM32H7xxUartDevice::RxAsyncForever(uint8_t *readBuffer, uint32_t bufferSize, WaitHandle *callback) {
    if (_handle->hdmarx->Init.Mode != DMA_CIRCULAR) {
        return DEVICE_STATUS::NOT_SUPPORT;
    }
    if ((HAL_UART_GetState(_handle) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX) {
        return DEVICE_STATUS::BUSY;
    }
    _rxCallback = callback;
    //TODO: enable it DMA_RXs
    if (HAL_UART_Receive_DMA(_handle, readBuffer, bufferSize) != HAL_OK) {
        callback->Error();
        return DEVICE_STATUS::HARDWARE_ERROR;
    }
    return DEVICE_STATUS::OK;
}

DEVICE_STATUS STM32H7xxUartDevice::RxAsyncForeverSilently(uint8_t *readBuffer, uint32_t bufferSize) {
    if (_handle->hdmarx->Init.Mode != DMA_CIRCULAR) {
        return DEVICE_STATUS::NOT_SUPPORT;
    }
    if ((HAL_UART_GetState(_handle) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX) {
        return DEVICE_STATUS::BUSY;
    }
    _rxCallback = nullptr;
    //TODO: disble it DMA_RXs
    if (HAL_UART_Receive_DMA(_handle, readBuffer, bufferSize) != HAL_OK) {
        LL_USART_DisableDMAReq_RX(_handle->Instance);
        return DEVICE_STATUS::HARDWARE_ERROR;
    }
    return DEVICE_STATUS::OK;
}

DEVICE_STATUS STM32H7xxUartDevice::StopRx() {
    if ((HAL_UART_GetState(_handle) & HAL_UART_STATE_BUSY_RX) != HAL_UART_STATE_BUSY_RX) {
        return DEVICE_STATUS::NOT_NECESSARY;
    }
    if (HAL_UART_DMAStop(_handle) != HAL_OK) {
        return DEVICE_STATUS::HARDWARE_ERROR;
    }
    _rxCallback = nullptr;
    return DEVICE_STATUS::OK;
}

uint32_t STM32H7xxUartDevice::GetRxDataPosition() {
    return _handle->RxXferSize - _handle->RxXferCount;
};

void STM32H7xxUartDevice::TxNotify() {
    if (_txCallback) {
        auto sync = _txCallback;
        _txCallback = nullptr;
        sync->Finish();
    }
};

void STM32H7xxUartDevice::RxNotify(uint32_t pos) {
    if (_rxCallback) {
        _rxCallback->SetPayload(reinterpret_cast<void *>(pos));
        auto sync = _rxCallback;
        _rxCallback = nullptr;
        sync->Finish();
    }
}

void STM32H7xxUartDevice::ErrorNotify() {

    if (_txCallback) {
        auto sync = _txCallback;
        _txCallback = nullptr;
        sync->Error();

    }
    if (_rxCallback) {
        auto sync = _rxCallback;
        _rxCallback = nullptr;
        sync->Error();

    }
}

DEVICE_STATUS STM32H7xxUartDevice::Tx(uint8_t *writeData, uint32_t dataSize) {
    if ((HAL_UART_GetState(_handle) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX) {
        return DEVICE_STATUS::BUSY;
    }
    if (HAL_UART_Transmit(_handle, writeData, dataSize, HAL_MAX_DELAY) != HAL_OK) {
        return DEVICE_STATUS::GENERAL_ERROR;
    }
    return DEVICE_STATUS::OK;
}

DEVICE_STATUS STM32H7xxUartDevice::Rx(uint8_t *readBuffer, uint32_t bufferSize, uint32_t *count) {
    if ((HAL_UART_GetState(_handle) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX) {
        return DEVICE_STATUS::BUSY;
    }
    if (HAL_UART_Receive(_handle, readBuffer, bufferSize, HAL_MAX_DELAY) != HAL_OK) {
        return DEVICE_STATUS::GENERAL_ERROR;
    }
    *count = bufferSize;
    return DEVICE_STATUS::OK;
}







