//#include "uart_stream.hpp"
//namespace wibot::accessor
//{
//
//using namespace wibot::peripheral;
//
//UartStream::UartStream(UART &uart, RingBuffer &rx_buffer, EventGroup &events, uint32_t doneFlag,
//                       uint32_t errFlag)
//    : _uart(uart), _rxBuffer(rx_buffer), _rxServerWaitHandler(events, doneFlag, errFlag){};
//
//Result UartStream::_init()
//{
//    INIT_BEGIN()
//    MEMBER_INIT_ERROR_CHECK(_uart)
//    _rxServerWaitHandler.done_callback_set(this, &UartStream::_rx_done_callback);
//    _rxServerWaitHandler.error_callback_set(this, &UartStream::_rx_error_callback);
//    INIT_END()
//};
//void UartStream::_deinit(){MEMBER_DEINIT(_uart)};
//
//void UartStream::_rx_done_callback(void *sender, void *event, void *receiver)
//{
//    UartStream *stream = (UartStream *)receiver;
//    uint16_t pos = (uint16_t)(uint32_t)event;
//    stream->rxBuffer_get().write_index_sync(pos);
//    if (stream->_rxWaitHandler != nullptr)
//    {
//        stream->_rxWaitHandler->done_set(stream);
//    }
//};
//void UartStream::_rx_error_callback(void *sender, void *event, void *receiver)
//{
//    UartStream *stream = (UartStream *)receiver;
//    stream->server_stop();
//};
//
//Result UartStream::server_start(WaitHandler &waitHandler)
//{
//    _rxWaitHandler = &waitHandler;
//    return _uart.start((uint8_t *)_rxBuffer.data_ptr_get(), _rxBuffer.mem_size_get(),
//                       _rxServerWaitHandler);
//};
//
//Result UartStream::server_stop()
//{
//    _rxWaitHandler = nullptr;
//    return _uart.stop();
//};
//
//Result UartStream::send(uint8_t *data, uint32_t dataSize, WaitHandler &waitHandler)
//{
//    return _uart.write(data, dataSize, waitHandler);
//};
//
//RingBuffer &UartStream::rxBuffer_get()
//{
//    return _rxBuffer;
//};
//
//} // namespace wibot::accessor
