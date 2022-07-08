#ifndef __STREAM_H__
#define __STREAM_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "device.h"
#include "ringbuffer.h"
#include "os/os.h"

    typedef struct Stream
    {
        UartDevice *device;
        RingBuffer *rxBuffer;
        DRIVER_EVENTS events;

        void (*onTxComplete)(struct Stream *stream);

        void (*onRxReady)(struct Stream *stream);

        void (*onError)(struct Stream *stream, OP_RESULT error);

    } Stream;

    OP_RESULT stream_create(Stream *stream, UartDevice *device, RingBuffer *rxBuffer);

    OP_RESULT stream_server_start(Stream *stream);

    OP_RESULT stream_server_stop(Stream *stream);

    OP_RESULT stream_send(Stream *stream, uint8_t *data, uint32_t size);
    /**
     * @brief 阻塞等待tx完成.
     *
     * @param stream
     * @return OP_RESULT
     */
    OP_RESULT stream_send_cplt_wait(Stream *stream, uint32_t timeout);
    /**
     * @brief 阻塞等待rx准备数据.
     *
     * @param stream
     * @param timeout
     * @return OP_RESULT
     */
    OP_RESULT stream_receive_ready_wait(Stream *stream, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif // __STREAM_H__
