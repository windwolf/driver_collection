//#ifndef __UART_HPP__
//#define __UART_HPP__
//
//#include "ring_buffer.hpp"
//#include "stdint.h"
//#include "common/shared.hpp"
//#include "common/driver.hpp"
//
//namespace Windwolf::Drivers
//{
//    using namespace Windwolf::Drivers;
//    using namespace Windwolf::Common;
//
//    struct UartConfig
//    {
//        enum class DATA_BIT
//        {
//            bit7,
//            bit8
//        };
//        enum class STOP_BIT
//        {
//            bit0_5,
//            bit1,
//            bit1_5,
//            bit2,
//        };
//        enum class SAMPLING
//        {
//            s8,
//            s16,
//        };
//        enum class Parity
//        {
//            even,
//            odd,
//            none
//        };
//        enum class Mode
//        {
//            tx,
//            rx,
//            tx_rx,
//        };
//        uint32_t baudRate;
//        DATA_BIT dataBit;
//        STOP_BIT stopBit;
//        Parity party;
//        SAMPLING sampling;
//        Mode mode;
//    };
//
//    class UartDeviceDriver : public DeviceDriver<UartConfig>, public AsyncIo
//    {
//    private:
//    public:
//    };
//
//    class UartDriver : public Driver
//    {
//    private:
//        RingBuffer<uint8_t> _txBuf;
//        RingBuffer<uint8_t> _rxBuf;
//
//    public:
//        UartDevice(UartDeviceDriver *driver, uint8_t *txBufferData, uint32_t txBufferSize, uint8_t *rxBufferData, uint32_t rxBufferSize)
//            : Device<UartConfig, UartDeviceDriver>(driver), _txBuf(txBufferData, txBufferSize), _rxBuf(rxBufferData, rxBufferSize)
//        {
//        }
//    };
//
//} // namespace Windwolf::Driver
//
//#endif // __UART_HPP__
