#ifndef ___STREAM_DEVICE_H__
#define ___STREAM_DEVICE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "basic/device.h"

    typedef struct StreamDevice
    {
        DeviceBase base;
        void *instance;
        void (*Init)(struct StreamDevice *device);
        DEVICE_STATUS(*StartRx)
        (struct StreamDevice *device);
        DEVICE_STATUS(*StopRx)
        (struct StreamDevice *device);
        DEVICE_STATUS(*Tx)
        (struct StreamDevice *device, uint8_t *data, uint32_t size);

        /**
         * @brief tx完成后的回调. @note 回调运行在中断上下文中, 注意控制时间. 
         * 
         */
        void (*onTxCompleteCallback)(struct StreamDevice *device);
        /**
         * @brief rx有值时的回调. @note 回调运行在中断上下文中, 注意控制时间. 
         * 
         */
        void (*onRxReadyCallback)(struct StreamDevice *device, uint16_t pos);
        void (*onErrorCallback)(struct StreamDevice *device);
    } StreamDevice;

#ifdef __cplusplus
}
#endif

#endif // ___STREAM_DEVICE_H__