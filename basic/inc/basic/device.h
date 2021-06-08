#ifndef __BASIC_DEVICE_H__
#define __BASIC_DEVICE_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"
#include "basic/shared.h"

#define DEVICE_STATUS uint32_t

#define DEVICE_STATUS_HARDWARE_ERROR -3
#define DEVICE_STATUS_ARGUMENT_ERROR -2
#define DEVICE_STATUS_GENERAL_ERROR -1

#define DEVICE_STATUS_OK 0
#define DEVICE_STATUS_NOT_NECESSARY 1
#define DEVICE_STATUS_NOT_READY 2
#define DEVICE_STATUS_BUSY 3
#define DEVICE_STATUS_NOT_SUPPORT 4

#define DEVICE_DATAWIDTH_8 0x00
#define DEVICE_DATAWIDTH_16 0x01
#define DEVICE_DATAWIDTH_24 0x02
#define DEVICE_DATAWIDTH_32 0x03

#define ALIGN(n) __attribute__((aligned(n)))

    typedef struct DeviceBase
    {
        void *host;
    } DeviceBase;

    void device_base_host_register(DeviceBase *device, void* host);

#ifdef __cplusplus
}
#endif

#endif // __BASIC_DEVICE_H__