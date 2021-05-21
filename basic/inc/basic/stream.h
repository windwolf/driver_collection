#ifndef __STREAM_H__
#define __STREAM_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "basic/device.h"
#include "basic/ring_buffer8.h"
#include "tx_api.h"

    typedef struct StreamDevice
    {
        void *instance;
        void (*Init)(struct StreamDevice *device);
        DEVICE_STATUS(*StartRx)
        (struct StreamDevice *device);
        DEVICE_STATUS(*StopRx)
        (struct StreamDevice *device);
        DEVICE_STATUS(*Tx)
        (struct StreamDevice *device, uint8_t *data, uint32_t size);
    } StreamDevice;

    typedef struct Stream
    {
        StreamDevice device;

        RingBuffer8 *rxBuffer;
        TX_EVENT_FLAGS_GROUP events;
        /**
         * @brief tx完成后的回调. @note 回调运行在中断上下文中, 注意控制时间. 
         * 
         */
        void (*onTxComplete)(struct Stream *stream);
        /**
         * @brief rx有值时的回调. @note 回调运行在中断上下文中, 注意控制时间. 
         * 
         */
        void (*onRxReady)(struct Stream *stream);
        void (*onError)(struct Stream *stream);
    } Stream;

    DEVICE_STATUS Stream_Init(Stream *stream, RingBuffer8 *rxBuffer);

    DEVICE_STATUS Stream_StartServer(Stream *stream);

    DEVICE_STATUS Stream_StopServer(Stream *stream);

    DEVICE_STATUS Stream_Tx(Stream *stream, uint8_t *data, uint32_t size);
    /**
     * @brief 阻塞等待tx完成.
     * 
     * @param stream 
     * @return DEVICE_STATUS 
     */
    UINT Stream_WaitForTxComplete(Stream *stream, ULONG timeout);
    /**
     * @brief 阻塞等待rx准备数据.
     * 
     * @param stream 
     * @param timeout 
     * @return DEVICE_STATUS 
     */
    UINT Stream_WaitForRxReady(Stream *stream, ULONG timeout);

    /**
     * @brief 底层设备调用
     * 
     * @param stream 
     */
    void Stream_DoTxComplete_(Stream *stream);

    void Stream_DoRxReady_(Stream *stream, uint16_t pos);

    void Stream_DoError_(Stream *stream);

#ifdef __cplusplus
}
#endif

#endif // __STREAM_H__
