#include "uart_stream.hpp"
namespace ww::accessor
{

using namespace ww::peripheral;

UartStream::UartStream(UART &uart, RingBuffer &rx_buffer)
    : _uart(uart), _rxBuffer(rx_buffer), _rxServerWaitHandler(this)
{
    _rxServerWaitHandler.done_callback_set(&UartStream::_rx_done_callback);
    _rxServerWaitHandler.error_callback_set(&UartStream::_rx_error_callback);
    initErrorCode = _uart.initErrorCode;
};

void UartStream::_rx_done_callback(void *sender, void *event, void *receiver)
{
    UartStream *stream = (UartStream *)receiver;
    uint16_t pos = (uint16_t)(uint32_t)event;
    stream->rxBuffer_get().write_index_sync(pos);
    if (stream->_rxWaitHandler != nullptr)
    {
        stream->_rxWaitHandler->done_set(stream);
    }
};
void UartStream::_rx_error_callback(void *sender, void *event, void *receiver)
{
    UartStream *stream = (UartStream *)receiver;
    stream->server_stop();
};

Result UartStream::server_start(WaitHandler &waitHandler)
{
    _rxWaitHandler = &waitHandler;
    return _uart.start((uint8_t *)_rxBuffer.data_ptr_get(),
                       _rxBuffer.mem_size_get(), _rxServerWaitHandler);
};

Result UartStream::server_stop()
{
    _rxWaitHandler = nullptr;
    return _uart.stop();
};

Result UartStream::send(uint8_t *data, uint32_t dataSize,
                        WaitHandler &waitHandler)
{
    return _uart.write(data, dataSize, waitHandler);
};

RingBuffer &UartStream::rxBuffer_get()
{
    return _rxBuffer;
};
} // namespace ww::accessor