#include "../inc/basic/device.h"

void device_base_host_register(DeviceBase *device, void *host)
{
    device->host = host;
};