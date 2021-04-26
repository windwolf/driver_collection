#ifndef __UART_DEVICE_DRIVER_HPP__
#define __UART_DEVICE_DRIVER_HPP__

#include "common\shared.hpp"
#include "common\uart.hpp"
#include "common\device.hpp"

#include "stm32h7xx_hal.h"
#include "bsp/uart_device_handle.h"


namespace Windwolf::Drivers::Bsp {
    using namespace Windwolf::Drivers;

    class STM32H7xxUartDevice : public IoDevice {
    private:
        UART_HandleTypeDef *_handle;
//        USART_HandleTypeDef *_usart;
//        enum class Mode {
//            uart,
//            usart,
//        };
//        Mode _mode;
        WaitHandle *_txSync;
        WaitHandle *_rxSync;

    public:
        STM32H7xxUartDevice(STM32H7xxUartDeviceHandle *handle);

//        STM32H7xxUartDevice(USART_HandleTypeDef *handle);
//        ~STM32H7xxUartDevice();

        virtual DEVICE_STATUS TxAsync(uint8_t *writeData, uint32_t dataSize, WaitHandle *txSync);

        virtual DEVICE_STATUS RxAsync(uint8_t *readBuffer, uint32_t bufferSize, WaitHandle *rxSync);

        virtual DEVICE_STATUS TxNotify();

        virtual DEVICE_STATUS RxNotify();
    };


};


#endif // __UART_DEVICE_DRIVER_HPP__
