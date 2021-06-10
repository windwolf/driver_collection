#include "../inc/bsp_pin.h"

DEVICE_STATUS pin_device_create(PinDevice *device, GPIO_TypeDef *handle, uint16_t pinMask, PIN_DEVICE_STATUS_INVERSE inverse)
{
    device_base_create((DeviceBase *)device);
    device->base.instance = handle;
    device->pinMask = pinMask;
    device->inverse = inverse;
    device->onTrigger = NULL;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS pin_device_init(PinDevice *device)
{
    return DEVICE_STATUS_OK;
};
DEVICE_STATUS pin_device_deinit(PinDevice *device)
{
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS pin_device_set(PinDevice *device, PIN_DEVICE_STATUS parent)
{
    HAL_GPIO_WritePin((GPIO_TypeDef *)device->base.instance, device->pinMask, parent ^ device->inverse);
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS pin_device_get(PinDevice *device, PIN_DEVICE_STATUS *parent)
{
    PIN_DEVICE_STATUS rst;
    rst = HAL_GPIO_ReadPin((GPIO_TypeDef *)device->base.instance, device->pinMask);
    *parent = rst ^ device->inverse;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS pin_device_toggle(struct PinDevice *device)
{
    HAL_GPIO_TogglePin((GPIO_TypeDef *)device->base.instance, device->pinMask);
    return DEVICE_STATUS_OK;
};
