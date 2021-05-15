#ifndef __BASIC_DEVICE_H__
#define __BASIC_DEVICE_H__

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum DEVICE_STATUS
    {

        DEVICE_STATUS_HARDWARE_ERROR = -3,
        DEVICE_STATUS_ARGUMENT_ERROR = -2,
        DEVICE_STATUS_GENERAL_ERROR = -1,

        DEVICE_STATUS_OK = 0,
        DEVICE_STATUS_NOT_NECESSARY = 1,
        DEVICE_STATUS_NOT_READY = 2,
        DEVICE_STATUS_BUSY = 3,
        DEVICE_STATUS_NOT_SUPPORT = 4,
    } DEVICE_STATUS;

    typedef struct P2pDevice
    {
        DEVICE_STATUS (*tx)(void);
    } P2pDevice;

    typedef struct BusDevice
    {
        
    } BusDevice;

    typedef struct Device
    {
        void *instance;
        void (*device_register)(struct Device *device);
    } Device;

#ifdef __cplusplus
}
#endif

#endif // __BASIC_DEVICE_H__