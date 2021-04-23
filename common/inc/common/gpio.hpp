// #ifndef __gpio_h__
// #define __gpio_h__

// #include "stdint.h"
// #include "common.hpp"

// namespace Windwolf::Drivers
// {

//     using namespace Windwolf::Drivers::Common;

//     struct GpioConfig
//     {
//         enum class GPIO_PORT
//         {
//             A,
//             B,
//             C,
//             D,
//             E,
//             F,
//             G,
//             H,
//             I,
//             J,
//             K,
//         };

//         enum class GPIO_DIRECTION
//         {
//             input,
//             output,
//             inputOutput,
//         };

//         enum class GPIO_SPEED
//         {
//             slow,
//             mid,
//             high,
//             veryHigh,
//         };
//         GPIO_PORT port;
//         GPIO_DIRECTION direction;
//         GPIO_SPEED speed;
//     };

//     class GpioDeviceDriver : public DeviceDriver<GpioConfig>
//     {
//     public:
//         virtual DEVICE_STATUS Set(uint8_t pin, bool value) = 0;
//         virtual DEVICE_STATUS Get(uint8_t pin, bool *value) = 0;
//     };

//     class GpioDevice : public Device<GpioConfig, GpioDeviceDriver>
//     {
//     public:
//         void Set(uint8_t pin, bool set);

//         bool Get(uint8_t pin);
//     };

//     void GpioDevice::Set(uint8_t pin, bool value)
//     {
//         _driver->Set(pin, value);
//     }
//     bool GpioDevice::Get(uint8_t pin)
//     {
//         bool value;
//         _driver->Get(pin, &value);
//         return value;
//     }

// } // namespace Driver

// #endif // __gpio_h__
