#include "uart_stream.hpp"
namespace ww::accessor
{

using namespace ww::peripheral;

Result UartStream::init()
{
    // TODO: set buffer datawidth to 8bits
    _uart.init();
    return Result_OK;
};

Result UartStream::deinit()
{
    _uart.deinit();
    return Result_OK;
};
Result UartStream::server_start(CallbackWaitHandler &waitHandler)
{
    _rxWaitHandler = &waitHandler;
    _rxWaitHandler->done_callback_set(
        [](void *sender, void *event, void *receiver) {
            UartStream *stream = (UartStream *)receiver;
            uint16_t pos = (uint16_t)(uint32_t)event;
            stream->rxBuffer_get().write_index_sync(pos);
        });
    _rxWaitHandler->error_callback_set(
        [](void *sender, void *event, void *receiver) {
            UartStream *stream = (UartStream *)receiver;
            stream->server_stop();
        });
    return _uart.start((uint8_t *)_rxBuffer.data_ptr_get(),
                       _rxBuffer.mem_size_get(), waitHandler);
};

Result UartStream::server_stop()
{
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