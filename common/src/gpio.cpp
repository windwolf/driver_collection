#include "gpio.hpp"

using namespace Windwolf::Driver::Core;

GPIO_Device::GPIO_Device(GPIO_DeviceDriver *pDriver) : DeviceBase(pDriver)
{
}

DEVICE_STATUS GPIO_Device::Init(GPIO_Config *pConfig)
{
    return DeviceBase::Init(pConfig);
}
DEVICE_STATUS GPIO_Device::ReConfig(GPIO_Config *pConfig)
{
    return DeviceBase::ReConfig(pConfig);
}

void GPIO_Device::Set(uint8_t pin, bool value)
{
    GPIO_DeviceDriver *driver = static_cast<GPIO_DeviceDriver *>(this->_pDriver);
    driver->set(this->_pInstance, pin, value);
}
bool GPIO_Device::Get(uint8_t pin)
{
    bool value;
    GPIO_DeviceDriver *driver = static_cast<GPIO_DeviceDriver *>(this->_pDriver);
    driver->get(this->_pInstance, pin, &value);
    return value;
}
