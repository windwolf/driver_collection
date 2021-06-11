#ifndef __STREAM_H__
#define __STREAM_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "device.h"
#include "ringbuffer.h"
#include "tx_api.h"

    typedef struct Stream
    {
        UartDevice *device;
        RingBuffer *rxBuffer;
        TX_EVENT_FLAGS_GROUP events;

        void (*onTxComplete)(struct Stream *stream);

        void (*onRxReady)(struct Stream *stream);

        void (*onError)(struct Stream *stream, DEVICE_STATUS error);

    } Stream;

    DEVICE_STATUS stream_create(Stream *stream, UartDevice *device, RingBuffer *rxBuffer);

    DEVICE_STATUS stream_server_start(Stream *stream);

    DEVICE_STATUS stream_server_stop(Stream *stream);

    DEVICE_STATUS stream_send(Stream *stream, uint8_t *data, uint32_t size);
    /**
     * @brief 阻塞等待tx完成.
     * 
     * @param stream 
     * @return DEVICE_STATUS 
     */
    UINT stream_send_cplt_wait(Stream *stream, ULONG timeout);
    /**
     * @brief 阻塞等待rx准备数据.
     * 
     * @param stream 
     * @param timeout 
     * @return DEVICE_STATUS 
     */
    UINT stream_receive_ready_wait(Stream *stream, ULONG timeout);

#ifdef __cplusplus
}
#endif

#endif // __STREAM_H__
