#include "../inc/common/device.h"

void device_base_create(DeviceBase *device)
{
    device->onError = NULL;
};

void _device_base_register(DeviceBase *device, void *parent,
                           DeviceBaseEventHandlerFuncType onError)
{
    device->parent = parent;
    device->onError = onError;
};

void _pin_device_register(PinDevice *device, void *parent,
                          PinDeviceEventHandlerFuncType onTrigger,
                          DeviceBaseEventHandlerFuncType onError)
{
    _device_base_register((DeviceBase *)device, parent, onError);
    device->onTrigger = onTrigger;
};
void _spi_device_register(SpiDevice *device, void *parent,
                          SpiDeviceEventHandlerFuncType onTxComplete,
                          SpiDeviceEventHandlerFuncType onRxComplete,
                          DeviceBaseEventHandlerFuncType onError)
{
    _device_base_register((DeviceBase *)device, parent, onError);
    device->onTxComplete = onTxComplete;
    device->onRxComplete = onRxComplete;
};

void _i2c_device_register(I2CDevice *device, void *parent,
                          I2CMemDeviceEventHandlerFuncType onWriteComplete,
                          I2CMemDeviceEventHandlerFuncType onReadComplete,
                          DeviceBaseEventHandlerFuncType onError)
{
    _device_base_register((DeviceBase *)device, parent, onError);
    device->onReadComplete = onReadComplete;
    device->onWriteComplete = onWriteComplete;
};

void _uart_device_register(UartDevice *device, void *parent,
                           UartDeviceEventHandlerFuncType onTxComplete,
                           UartDeviceRxEventHandlerFuncType onRxComplete,
                           DeviceBaseEventHandlerFuncType onError)
{
    _device_base_register((DeviceBase *)device, parent, onError);
    device->onTxComplete = onTxComplete;
    device->onRxComplete = onRxComplete;
};

void _sd_device_register(SdDevice *device, void *parent,
                         SdDeviceEventHandlerFuncType onTxComplete,
                         SdDeviceEventHandlerFuncType onRxComplete,
                         DeviceBaseEventHandlerFuncType onError)
{
    _device_base_register((DeviceBase *)device, parent, onError);
    device->onTxComplete = onTxComplete;
    device->onRxComplete = onRxComplete;
};