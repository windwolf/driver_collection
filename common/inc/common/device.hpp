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

    class IoDevice //: public Device
    {
        struct MODE {
            uint8_t txSync: 1;
            uint8_t txAsync: 1;
            uint8_t rxSync: 1;
            uint8_t rxAsync: 1;
            uint8_t rxLoop: 1;
        };

    private:
        Callback<void, size_t> _txCallback;
        Callback<void, Buffer<uint8_t>> _rxCallback;
        MODE mode;

    public:
        IoDevice() : _txCallback(), _rxCallback() {};

//        virtual bool supportRxNotification() {
//            return (_txCallback != nullptr);
//        }
//
//        virtual bool supportTxNotification() {
//            return (_rxCallback != nullptr);
//        }

        virtual DEVICE_STATUS RegisterTxCompleteNotification(Callback<void, size_t> txCallback) {
            _txCallback = txCallback;
            return DEVICE_STATUS::OK;
        }

        virtual DEVICE_STATUS RegisterRxCompleteNotification(Callback<void, Buffer<uint8_t> > rxCallback) {
            _rxCallback = rxCallback;
            return DEVICE_STATUS::OK;
        }

        virtual DEVICE_STATUS Tx(uint8_t *writeData, uint32_t dataSize) = 0;

        virtual DEVICE_STATUS Rx(uint8_t *readBuffer, uint32_t bufferSize) = 0;

        virtual DEVICE_STATUS TxAsync(uint8_t *writeData, uint32_t dataSize) = 0;

        virtual DEVICE_STATUS RxAsync(uint8_t *readBuffer, uint32_t bufferSize) = 0;

        virtual DEVICE_STATUS StartRxLoop(const Buffer<uint8_t> &buffer) = 0;

        virtual DEVICE_STATUS StopRxLoop() = 0;
    };

} // namespace Windwolf::Driver

#endif //STM32_THREADX_STARTUP_DEVICE_H
