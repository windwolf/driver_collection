//
// Created by windwolf on 2021/4/15.
//
#pragma once
#ifndef STM32_THREADX_STARTUP_DRIVER_H
#define STM32_THREADX_STARTUP_DRIVER_H

#include "common/device.hpp"
#include "common/ring_buffer.hpp"

namespace windwolf::drivers {
    using namespace windwolf::common;
#define STREAM_DRIVER_SERVER_STACK_SIZE 2048

    struct StreamDriver {
        enum class Status {
            initial,
            opened,
            closed,
        };

        IoDevice &_device;
        Status _status;
        RingBuffer<uint8_t> _txBuffer;
        RingBuffer<uint8_t> _rxBuffer;
        WaitHandle<void *> _txWaitHandle;
        WaitHandle<Buffer2<uint8_t>> _rxWaitHandle;
        OsThread &_worker;
        bool _markAsStop;

        StreamDriver(IoDevice &device, OsThread &worker, Buffer2<uint8_t> txBuf, Buffer2<uint8_t> rxBuf, OsEvent &txEvent, OsEvent &rxEvent);

        /**
         * @brief Start the device to get ready for transmition or receiving.
         *
         * For device support DMA or IT, it register a callback to device to monitor the tx buffer empty or rx buffer full event.
         * For device not support polling, it start loop routine, in that hardware fills the rx ringbuffer.
         *
         * @return bool
         */
        bool Open();

        bool Close();

        DEVICE_STATUS Write(Buffer2<uint8_t> data);

        DEVICE_STATUS Read(Buffer2<uint8_t> data);

        /**
         * @brief Notify driver that rxbuffer is not empty, when new data received.
         *
         * Called by ISR, etc. USART_IDLE, DMA_HALF_COMPLETE, DMA_COMPLATE.
         *
         * @return DEVICE_STATUS
         */
        DEVICE_STATUS NotifyRxReady();
    };

} // namespace windwolf::Driver

#endif //STM32_THREADX_STARTUP_DRIVER_H
