#ifndef __gpio_h__
#define __gpio_h__

#include "stdint.h"
#include "common.hpp"

namespace Windwolf::Driver::Core {
    class GPIO_Device;

    class GPIO_DeviceDriver : public DeviceDriverBase {
    public:
        virtual DEVICE_STATUS set(void *pInstance, uint8_t pin, bool value) = 0;

        virtual DEVICE_STATUS get(void *pInstance, uint8_t pin, bool *value) = 0;
    };

    struct GPIO_Config {
        enum GPIO_PORT {
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
        };

        enum GPIO_DIRECTION {
            input,
            output,
            inputOutput,
        };

        enum GPIO_SPEED {
            slow,
            mid,
            high,
            veryHigh,
        };
        GPIO_PORT port;
        GPIO_DIRECTION direction;
        GPIO_SPEED speed;
    };

    class GPIO_Device : public DeviceBase {
    public:

        GPIO_Device(GPIO_DeviceDriver *pDriver);

        DEVICE_STATUS Init(GPIO_Config *pConfig);

        DEVICE_STATUS ReConfig(GPIO_Config *pConfig);

        void Set(uint8_t pin, bool set);

        bool Get(uint8_t pin);
    };

} // namespace Driver

#endif // __gpio_h__
