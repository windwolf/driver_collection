#ifndef __WWDEVICE_ACCESSOR_UART_STREAM_HPP__
#define __WWDEVICE_ACCESSOR_UART_STREAM_HPP__

#include "ringbuffer.hpp"
#include "soc.hpp"

namespace ww::accessor
{

using namespace ww::peripheral;

class UartStream
{
  public:
    UartStream(UART &uart, RingBuffer &rx_buffer)
        : _uart(uart), _rxBuffer(rx_buffer), _rxServerWaitHandler(this)
    {
        _rxServerWaitHandler.done_callback_set(&UartStream::_rx_done_callback);
        _rxServerWaitHandler.error_callback_set(
            &UartStream::_rx_error_callback);
    };
    Result init();
    Result deinit();
    Result server_start(WaitHandler &waitHandler);

    Result server_stop();

    Result send(uint8_t *data, uint32_t dataSize, WaitHandler &waitHandler);

    RingBuffer &rxBuffer_get();

  private:
    UART &_uart;
    RingBuffer &_rxBuffer;
    CallbackWaitHandler _rxServerWaitHandler;
    WaitHandler *_rxWaitHandler;
    static void _rx_done_callback(void *sender, void *event, void *receiver);
    static void _rx_error_callback(void *sender, void *event, void *receiver);
}; // namespace ww::peripheralclassUartStream
} // namespace ww::accessor

#endif // __WWDEVICE_ACCESSOR_UART_STREAM_HPP__