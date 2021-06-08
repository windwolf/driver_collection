#ifndef ___PACKET_H__
#define ___PACKET_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "basic/device.h"
#include "basic/buffer.h"

    typedef enum PACKET_IO_DEVICE_OP_MODE
    {
        PACKET_IO_DEVICE_OP_MODE_SYNC = 0x1,
        PACKET_IO_DEVICE_OP_MODE_ASYNC = 0x2,
    } PACKET_IO_DEVICE_OP_MODE;

    struct PacketIoDevice;
    typedef void (*PacketIoDeviceEventHandlerFuncType)(struct PacketIoDevice *packetIo);
    typedef DEVICE_STATUS (*PacketIoDeviceTxFuncType)(struct PacketIoDevice *device, void *data, uint32_t size, uint8_t width);
    typedef DEVICE_STATUS (*PacketIoDeviceRxFuncType)(struct PacketIoDevice *device, void *buffer, uint32_t size, uint8_t width, uint8_t dummyCycleCount);

    typedef struct PacketIoDevice
    {
        DeviceBase base;
        void *instance;
        PACKET_IO_DEVICE_OP_MODE opMode;
        Buffer _rxBuffer;

        void (*Init)(struct PacketIoDevice *device);

        PacketIoDeviceTxFuncType Tx;
        PacketIoDeviceRxFuncType Rx;
        PacketIoDeviceTxFuncType TxAsync;
        PacketIoDeviceRxFuncType RxAsync;

        // DEVICE_STATUS(*TxN8)
        // (struct PacketIoDevice *device, uint8_t *data, uint32_t size);
        // DEVICE_STATUS(*RxN8)
        // (struct PacketIoDevice *device, uint8_t *buffer, uint32_t size, uint8_t dummyCycleCount);
        // DEVICE_STATUS(*TxN8Async)
        // (struct PacketIoDevice *device, uint8_t *data, uint32_t size);
        // DEVICE_STATUS(*RxN8Async)
        // (struct PacketIoDevice *device, uint8_t *buffer, uint32_t size, uint8_t dummyCycleCount);

        // DEVICE_STATUS(*TxN16)
        // (struct PacketIoDevice *device, uint16_t *data, uint32_t size);
        // DEVICE_STATUS(*RxN16)
        // (struct PacketIoDevice *device, uint16_t *buffer, uint32_t size, uint8_t dummyCycleCount);
        // DEVICE_STATUS(*TxN16Async)
        // (struct PacketIoDevice *device, uint16_t *data, uint32_t size);
        // DEVICE_STATUS(*RxN16Async)
        // (struct PacketIoDevice *device, uint16_t *buffer, uint32_t size, uint8_t dummyCycleCount);

        PacketIoDeviceEventHandlerFuncType onTxComplete;
        PacketIoDeviceEventHandlerFuncType onRxComplete;
        PacketIoDeviceEventHandlerFuncType onError;
    } PacketIoDevice;

    void packet_io_device_host_register(PacketIoDevice *device, void *host,
                                        PacketIoDeviceEventHandlerFuncType onTxComplete,
                                        PacketIoDeviceEventHandlerFuncType onRxComplete,
                                        PacketIoDeviceEventHandlerFuncType onError);

#ifdef __cplusplus
}
#endif

#endif // ___PACKET_H__