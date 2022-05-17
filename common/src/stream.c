#include "../inc/common/stream.h"

#define STREAM_EVENT_RX_READY 0b0001
#define STREAM_EVENT_TX_COMPLETE 0b0010
#define STREAM_EVENT_TX_READY 0b0100

static inline void _stream_tx_complete(Stream *stream)
{
    if (stream->onTxComplete)
    {
        stream->onTxComplete(stream);
    }
    driver_events_set(&(stream->events), STREAM_EVENT_TX_READY);
};

static inline void _stream_rx_ready(Stream *stream, uint16_t pos)
{
    ringbuffer_write_index_sync(stream->rxBuffer, pos);

    if (stream->onRxReady)
    {
        stream->onRxReady(stream);
    }
    driver_events_set(&(stream->events), STREAM_EVENT_RX_READY);
};

static inline void _stream_error(Stream *stream, OP_RESULT error)
{
    driver_events_reset(&(stream->events), STREAM_EVENT_RX_READY);
    if (stream->onError)
    {
        stream->onError(stream, error);
    }
};

static inline void _stream_device_tx_cplt(UartDevice *device)
{
    _stream_tx_complete((Stream *)device->base.parent);
}
static inline void _stream_device_rx_cplt(UartDevice *device, uint16_t pos)
{
    _stream_rx_ready((Stream *)device->base.parent, pos);
}
static inline void _stream_device_error(DeviceBase *device, OP_RESULT error)
{
    _stream_error((Stream *)device->parent, error);
}

OP_RESULT stream_create(Stream *stream, UartDevice *device, RingBuffer *rxBuffer)
{
    stream->device = device;
    stream->rxBuffer = rxBuffer;
    driver_events_create(&(stream->events), "stream");
    driver_events_set(&(stream->events), STREAM_EVENT_TX_READY);
    _uart_device_register(device, stream,
                          &_stream_device_tx_cplt,
                          &_stream_device_rx_cplt,
                          &_stream_device_error);
    return OP_RESULT_OK;
}

OP_RESULT stream_server_start(Stream *stream)
{
    return uart_device_circular_rx_start(stream->device, stream->rxBuffer->data, stream->rxBuffer->size);
}

OP_RESULT stream_server_stop(Stream *stream)
{
    return uart_device_circular_rx_stop(stream->device);
}

OP_RESULT stream_send(Stream *stream, uint8_t *data, uint32_t size)
{
    driver_events_reset(&(stream->events), STREAM_EVENT_TX_READY);
    return uart_device_tx(stream->device, data, size);
}

OP_RESULT stream_send_cplt_wait(Stream *stream, uint32_t timeout)
{
    return driver_events_get(&stream->events, STREAM_EVENT_TX_READY, DRIVER_EVENTS_OPTION_AND, timeout);
}
/**
 * @brief 阻塞等待rx准备数据.
 *
 * @param stream
 * @param timeout
 * @return DEVICE_STATUS
 */
OP_RESULT stream_receive_ready_wait(Stream *stream, uint32_t timeout)
{
    if (driver_events_get(&stream->events, STREAM_EVENT_RX_READY, DRIVER_EVENTS_OPTION_AND, timeout))
    {
        return OP_RESULT_OK;
    }
    else
    {
        return OP_RESULT_TIMEOUT;
    }
}