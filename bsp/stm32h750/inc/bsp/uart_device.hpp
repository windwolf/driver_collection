#ifndef __UART_DEVICE_DRIVER_HPP__
#define __UART_DEVICE_DRIVER_HPP__

#include "common\shared.hpp"
#include "common\uart.hpp"
#include "common\device.hpp"

#include "stm32h7xx_hal.h"


namespace Windwolf::Drivers::Bsp {
    using namespace Windwolf::Drivers;
    enum class UartIndex {
        UART4_INDEX = 0,
        UART5_INDEX,
        UART7_INDEX,
        UART8_INDEX,
        UART_COUNT,
    };

    class STM32H7xxUartDevice /* : public IoDevice */ {
    private:
        UART_HandleTypeDef *_handle;
//        USART_HandleTypeDef *_usart;
//        enum class Mode {
//            uart,
//            usart,
//        };
//        Mode _mode;
        WaitHandle *_txCallback;
        WaitHandle *_rxCallback;
        bool _rxForever;
    public:


        STM32H7xxUartDevice(UART_HandleTypeDef *handle);

//        STM32H7xxUartDevice(USART_HandleTypeDef *handle);
//        ~STM32H7xxUartDevice();

        virtual DEVICE_STATUS Tx(uint8_t *writeData, uint32_t dataSize);

        virtual DEVICE_STATUS Rx(uint8_t *readBuffer, uint32_t bufferSize, uint32_t *count);

        /**
         * Send data async, return immediately.
         * callback function will be called, when sending process done.
         * @param writeData
         * @param dataSize
         * @param callback
         * @return
         */
        virtual DEVICE_STATUS TxAsync(uint8_t *writeData, uint32_t dataSize, WaitHandle *callback);


        /**
         * receive data async, return immediately.
         * Callback function will be called, when receive buffer full or rx line idle.
         * @param readBuffer
         * @param bufferSize
         * @param callback
         * @return
         */
        virtual DEVICE_STATUS RxAsync(uint8_t *readBuffer, uint32_t bufferSize, WaitHandle *callback);

        /**
         * receive data async and circlly, return immediately.
         * Callback function will be called, when receive buffer reach the end of the buffer or rx line idle.
         * @param readBuffer
         * @param bufferSize
         * @param callback
         * @return
         */
        virtual DEVICE_STATUS RxAsyncForever(uint8_t *readBuffer, uint32_t bufferSize, WaitHandle *callback);

        /**
         * Start rx process with on read buffer, return immediately.
         * Nothing will happen when receive data is available. User code should get the data head position and mentain the buffer.
         * @param readBuffer
         * @param bufferSize
         * @return
         */
        virtual DEVICE_STATUS RxAsyncForeverSilently(uint8_t *readBuffer, uint32_t bufferSize);

        /**
         * Stop the rx process.
         * @return
         */
        virtual DEVICE_STATUS StopRx();

        /**
         * Get the rx data head position.
         * @return
         */
        virtual uint32_t GetRxDataPosition();

        /**
         * Call by tx interept handle, to notify the tx event.
         * @return
         */
        virtual void TxNotify();

        /**
         * Call by rx interept handle, to notify the rx event.
         * @return
         */
        virtual void RxNotify(uint32_t pos);

        virtual void ErrorNotify();
    };


};


#endif // __UART_DEVICE_DRIVER_HPP__
