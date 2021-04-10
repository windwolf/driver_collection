#include "gpio.hpp"

using namespace Driver::Core;

DEVICE_STATUS GPIO_Device::Init(GPIO_Config *pConfig)
{
    
}
DEVICE_STATUS GPIO_Device::ReConfig(GPIO_Config *pConfig)
{
}

void GPIO_Init(GPIO_INSTANCE *ins, GPIO_DRIVERS *drivers)

{
    ins->drivers = drivers;
}

void GPIO_DeInit(GPIO_INSTANCE *ins);

void GPIO_Config(GPIO_INSTANCE *ins, GPIO_CONFIG config);

void GPIO_Set(GPIO_INSTANCE *ins, uint8_t pin, uint8_t setOrReset);

uint8_t GPIO_Get(GPIO_INSTANCE *ins, uint8_t pin);