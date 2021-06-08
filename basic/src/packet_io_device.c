#include "../inc/basic/packet_io_device.h"

void packet_io_device_host_register(PacketIoDevice *device, void *host,
                                    PacketIoDeviceEventHandlerFuncType onTxComplete,
                                    PacketIoDeviceEventHandlerFuncType onRxComplete,
                                    PacketIoDeviceEventHandlerFuncType onError)
{
    device_base_host_register(device, host);
    device->onTxComplete = onTxComplete;
    device->onRxComplete = onRxComplete;
    device->onError = onError;
}
