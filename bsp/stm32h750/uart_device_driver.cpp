// #include "bsp/uart_device_driver.hpp"

// using namespace Windwolf::Drivers::Bsp;
// using namespace Windwolf::Drivers;

// #ifdef __cplusplus
// extern "C"
// {
// #endif

//     static STM32H750UartDeviceDriver *usart1;
//     static STM32H750UartDeviceDriver *usart2;
//     static STM32H750UartDeviceDriver *usart3;
//     static STM32H750UartDeviceDriver *usart6;
//     static STM32H750UartDeviceDriver *uart4;
//     static STM32H750UartDeviceDriver *uart5;
//     static STM32H750UartDeviceDriver *uart7;
//     static STM32H750UartDeviceDriver *uart8;

//     void Usart_TxCpltCallback__(struct __USART_HandleTypeDef *husart)
//     {
//         switch (reinterpret_cast<uint32_t>(husart->Instance))
//         {
//         case (uint32_t)(USART1_BASE):
//             usart1->NotifyWriteComplete();
//             break;
//         case (uint32_t)(USART2_BASE):
//             usart2->NotifyWriteComplete();
//             break;
//         case (uint32_t)(USART3_BASE):
//             usart3->NotifyWriteComplete();
//             break;
//         case (uint32_t)(USART6_BASE):
//             usart6->NotifyWriteComplete();
//             break;
//         default:
//             break;
//         };
//     }
//     void Uart_TxCpltCallback__(struct __UART_HandleTypeDef *huart)
//     {
//         switch (reinterpret_cast<uint32_t>(huart->Instance))
//         {
//         case (uint32_t)(UART4_BASE):
//             uart4->NotifyWriteComplete();
//             break;
//         case (uint32_t)(UART5_BASE):
//             uart5->NotifyWriteComplete();
//             break;
//         case (uint32_t)(UART7_BASE):
//             uart7->NotifyWriteComplete();
//             break;
//         case (uint32_t)(UART8_BASE):
//             uart8->NotifyWriteComplete();
//             break;
//         default:
//             break;
//         };
//     }

// #ifdef __cplusplus
// }
// #endif

// STM32H750UartDeviceDriver::STM32H750UartDeviceDriver(USART_HandleTypeDef *handle)
// {
//     _usart = handle;
//     _usart->TxCpltCallback = Usart_TxCpltCallback__;
//     _mode = Mode::usart;

//     switch (reinterpret_cast<uint32_t>(handle->Instance))
//     {
//     case (uint32_t)(USART1_BASE):
//         usart1 = this;
//         break;
//     case (uint32_t)(USART2_BASE):
//         usart2 = this;
//         break;
//     case (uint32_t)(USART3_BASE):
//         usart3 = this;
//         break;
//     case (uint32_t)(USART6_BASE):
//         usart6 = this;
//         break;
//     default:
//         break;
//     };
// }
// STM32H750UartDeviceDriver::STM32H750UartDeviceDriver(UART_HandleTypeDef *handle)
// {
//     _uart->TxCpltCallback = Uart_TxCpltCallback__;
//     _uart = handle;
//     _mode = Mode::uart;

//     switch (reinterpret_cast<uint32_t>(handle->Instance))
//     {
//     case (uint32_t)(UART4_BASE):
//         uart4 = this;
//         break;
//     case (uint32_t)(UART5_BASE):
//         uart5 = this;
//         break;
//     case (uint32_t)(UART7_BASE):
//         uart7 = this;
//         break;
//     case (uint32_t)(UART8_BASE):
//         uart8 = this;
//         break;
//     default:
//         break;
//     };
// }

// DEVICE_STATUS STM32H750UartDeviceDriver::Init()
// {
//     switch (_mode)
//     {
//     case Mode::usart:
//         HAL_USART_Init(this->_usart);
//         break;
//     case Mode::uart:
//         HAL_UART_Init(this->_uart);
//         break;
//     default:
//         break;
//     }

//     return DEVICE_STATUS::OK;
// }
// DEVICE_STATUS STM32H750UartDeviceDriver::DeInit()
// {
//     switch (_mode)
//     {
//     case Mode::usart:
//         HAL_USART_DeInit(this->_usart);
//         break;
//     case Mode::uart:
//         HAL_UART_DeInit(this->_uart);
//         break;
//     default:
//         break;
//     }
//     return DEVICE_STATUS::OK;
// }
// DEVICE_STATUS STM32H750UartDeviceDriver::Config(UartConfig *config)
// {
//     switch (_mode)
//     {
//     case Mode::usart:
//         HAL_USART_DeInit(this->_usart);
//         HAL_USART_Init(this->_usart);
//         break;
//     case Mode::uart:
//         HAL_UART_DeInit(this->_uart);
//         HAL_UART_Init(this->_uart);
//         break;
//     default:
//         break;
//     };
//     return DEVICE_STATUS::OK;
// }
// DEVICE_STATUS STM32H750UartDeviceDriver::WriteAsync(uint8_t *writeData, uint32_t dataSize, WaitHandle *sync)
// {
//     switch (_mode)
//     {
//     case Mode::usart:
//         _txSync = sync;
//         HAL_USART_Transmit_DMA(_usart, writeData, dataSize);
//         break;
//     case Mode::uart:
//         break;
//     default:
//         break;
//     };
//     return DEVICE_STATUS::OK;
// }
// DEVICE_STATUS STM32H750UartDeviceDriver::ReadAsync(uint8_t *readBuffer, uint32_t bufferSize, WaitHandle *sync)
// {
//     return DEVICE_STATUS::OK;
// }
// void STM32H750UartDeviceDriver::NotifyWriteComplete()
// {
//     _txSync->Finish();
//     _txSync = nullptr;
// }
