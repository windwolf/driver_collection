// #ifndef __WWDEVICE_ACCESSOR_UART_STREAM_HPP__
// #define __WWDEVICE_ACCESSOR_UART_STREAM_HPP__
//
// #include "ringbuffer.hpp"
// #include "soc.hpp"
//
// namespace wibot::accessor
//{
//
// using namespace wibot::peripheral;
//
// class UartStream : public Initializable
//{
//   public:
//     UartStream(UART &uart, RingBuffer &rx_buffer, EventGroup &events, uint32_t doneFlag,
//                uint32_t errFlag);
//     Result _init() override;
//     void _deinit() override;
//     Result server_start(WaitHandler &waitHandler);
//
//     Result server_stop();
//
//     Result send(uint8_t *data, uint32_t dataSize, WaitHandler &waitHandler);
//
//     RingBuffer &rxBuffer_get();
//
//   private:
//     UART &_uart;
//     RingBuffer &_rxBuffer;
//     WaitHandler _rxServerWaitHandler;
//     WaitHandler *_rxWaitHandler;
//     static void _rx_done_callback(void *sender, void *event, void *receiver);
//     static void _rx_error_callback(void *sender, void *event, void *receiver);
// }; // namespace wibot::peripheralclassUartStream
// } // namespace wibot::accessor
//
// #endif // __WWDEVICE_ACCESSOR_UART_STREAM_HPP__
