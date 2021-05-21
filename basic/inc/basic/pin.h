#ifndef ___PIN_H__
#define ___PIN_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "basic/device.h"

    typedef enum PIN_STATUS
    {
        PIN_STATUS_RESET = 0U,
        PIN_STATUS_SET
    } PIN_STATUS;

    typedef struct PinDevice
    {
        void *instance;
        void (*Init)(struct PinDevice *device);
        DEVICE_STATUS(*Write)
        (struct PinDevice *device, uint16_t pinMask, uint8_t value);
        DEVICE_STATUS(*Read)
        (struct PinDevice *device, uint16_t pinMask, uint16_t *value);
        DEVICE_STATUS(*Toggle)
        (struct PinDevice *device, uint16_t pinMask);
    } PinDevice;

    typedef struct Pin
    {
        PinDevice device;
        uint16_t pinMask;
    } Pin;

    DEVICE_STATUS Pin_Init(Pin *pin);

    DEVICE_STATUS Pin_Set(Pin *pin, PIN_STATUS state);
    DEVICE_STATUS Pin_Get(Pin *pin, uint16_t *value);
    DEVICE_STATUS Pin_Toggle(Pin *pin);

#ifdef __cplusplus
}
#endif

#endif // ___PIN_H__