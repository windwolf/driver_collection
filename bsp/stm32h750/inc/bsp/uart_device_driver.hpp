#ifndef __UART_DEVICE_DRIVER_HPP__
#define __UART_DEVICE_DRIVER_HPP__

#include "common/shared.hpp"
#include "common/uart.hpp"
#include "common/device.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32h7xx_hal.h"

#ifdef __cplusplus
}
#endif

namespace Windwolf::Drivers::Bsp
{
    using namespace Windwolf::Drivers;
    struct STM32H750UartDevice : public IoDevice
    {
    private:
        UART_HandleTypeDef *_uart;
        USART_HandleTypeDef *_usart;
        enum class Mode
        {
            uart,
            usart,
        };
        Mode _mode;
        WaitHandle *_txSync;
        WaitHandle *_rxSync;

    public:
        STM32H750UartDevice(UART_HandleTypeDef *handle);
        STM32H750UartDevice(USART_HandleTypeDef *handle);
        ~STM32H750UartDevice();
        virtual DEVICE_STATUS Init();
        virtual DEVICE_STATUS DeInit();
        virtual DEVICE_STATUS Config(UartConfig *config);
        virtual DEVICE_STATUS WriteAsync(uint8_t *writeData, uint32_t dataSize, WaitHandle *sync);
        virtual DEVICE_STATUS ReadAsync(uint8_t *readBuffer, uint32_t bufferSize, WaitHandle *sync);
        virtual void NotifyWriteComplete();
    };

} // namespace Windwolf::Driver::Bsp

#endif // __UART_DEVICE_DRIVER_HPP__