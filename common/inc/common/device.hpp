//
// Created by windwolf on 2021/4/15.
//
#pragma once
#ifndef STM32_THREADX_STARTUP_DEVICE_H
#define STM32_THREADX_STARTUP_DEVICE_H

#include "stdint.h"
#include "common/shared.hpp"

namespace Windwolf::Drivers {
    using namespace Windwolf::Common;

    class IoDevice {
        struct MODE {
            uint8_t txSync: 1;
            uint8_t txAsync: 1;
            uint8_t rxSync: 1;
            uint8_t rxAsync: 1;
            uint8_t rxLoop: 1;
        };

    public:

//        virtual DEVICE_STATUS Tx(uint8_t *writeData, uint32_t dataSize) = 0;
//
//        virtual DEVICE_STATUS Rx(uint8_t *readBuffer, uint32_t bufferSize) = 0;

        virtual DEVICE_STATUS TxAsync(uint8_t *writeData, uint32_t dataSize, WaitHandle *waitHandle) = 0;

        virtual DEVICE_STATUS RxAsync(uint8_t *readBuffer, uint32_t bufferSize, WaitHandle *waitHandle) = 0;

//        virtual DEVICE_STATUS RxForever(uint8_t *readBuffer, uint32_t bufferSize, WaitHandle *waitHandle) = 0;

        virtual DEVICE_STATUS TxNotify() = 0;

        virtual DEVICE_STATUS RxNotify() = 0;
    };

} // namespace Windwolf::Driver

#endif //STM32_THREADX_STARTUP_DEVICE_H
