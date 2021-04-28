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
            uint8_t rxAsyncCircle: 1;
        };

    public:

        virtual DEVICE_STATUS Tx(uint8_t *writeData, uint32_t dataSize) = 0;

        virtual DEVICE_STATUS Rx(uint8_t *readBuffer, uint32_t bufferSize) = 0;

        /**
         * Send data async, return immediately.
         * callback function will be called, when sending process done.
         * @param writeData
         * @param dataSize
         * @param callback
         * @return
         */
        virtual DEVICE_STATUS TxAsync(uint8_t *writeData, uint32_t dataSize, WaitHandle *waitHandle) = 0;


        /**
         * receive data async, return immediately.
         * Callback function will be called, when receive buffer full or rx line idle.
         * @param readBuffer
         * @param bufferSize
         * @param callback
         * @return
         */
        virtual DEVICE_STATUS RxAsync(uint8_t *readBuffer, uint32_t bufferSize, WaitHandle *callback) = 0;

        /**
         * receive data async and circlly, return immediately.
         * Callback function will be called, when receive buffer reach the end of the buffer or rx line idle.
         * @param readBuffer
         * @param bufferSize
         * @param callback
         * @return
         */
        virtual DEVICE_STATUS RxAsyncForever(uint8_t *readBuffer, uint32_t bufferSize, WaitHandle *callback) = 0;

        /**
         * Start rx process with on read buffer, return immediately.
         * Nothing will happen when receive data is available. User code should get the data head position and mentain the buffer.
         * @param readBuffer
         * @param bufferSize
         * @return
         */
        virtual DEVICE_STATUS RxAsyncForeverSilently(uint8_t *readBuffer, uint32_t bufferSize) = 0;

        /**
         * Stop the rx process.
         * @return
         */
        virtual DEVICE_STATUS StopRx() = 0;

        /**
         * Get the rx data head position.
         * @return
         */
        virtual uint32_t GetRxDataPosition() = 0;

        /**
         * Call by tx interept handle, to notify the tx event.
         * @return
         */
        virtual void TxNotify() = 0;

        /**
         * Call by rx interept handle, to notify the rx event.
         * @return
         */
        virtual void RxNotify(uint32_t pos) = 0;

        virtual void ErrorNotify() = 0;
    };

} // namespace Windwolf::Driver

#endif //STM32_THREADX_STARTUP_DEVICE_H
