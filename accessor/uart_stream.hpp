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
        : _uart(uart), _rxBuffer(rx_buffer){};
    Result init();
    Result deinit();
    Result server_start(CallbackWaitHandler &waitHandler);

    Result server_stop();

    Result send(uint8_t *data, uint32_t dataSize, WaitHandler &waitHandler);

    RingBuffer &rxBuffer_get();

  private:
    UART &_uart;
    RingBuffer &_rxBuffer;
    CallbackWaitHandler *_rxWaitHandler;
}; // namespace ww::peripheralclassUartStream
} // namespace ww::accessor

#endif // __WWDEVICE_ACCESSOR_UART_STREAM_HPP__