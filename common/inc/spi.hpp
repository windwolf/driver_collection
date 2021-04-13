//
// Created by windwolf on 2021/4/10.
//

#ifndef STARTUP_SPI_H
#define STARTUP_SPI_H

#include "stdint.h"
#include "common.hpp"
#include "gpio.hpp"
#include "os_port.hpp"
#include "wait_handle.hpp"

namespace Windwolf::Driver::Core
{
    using namespace Windwolf::Common;
    class SPI_DeviceDriver : public DeviceDriverBase
    {
    public:
        DEVICE_STATUS WriteIt(uint8_t *writeData, uint32_t dataSize);

        DEVICE_STATUS WriteDma(uint8_t *writeData, uint32_t dataSize);

        DEVICE_STATUS WriteReadIt(uint8_t *writeData, uint32_t dataSize, uint8_t *readBuffer, uint32_t bufferSize);

        DEVICE_STATUS WriteReadDma(uint8_t *writeData, uint32_t dataSize, uint8_t *readBuffer, uint32_t bufferSize);
    };

    struct SPI_Config
    {
        GPIO_Config::GPIO_PORT csPort;
        uint8_t csPin;
        GPIO_Config::GPIO_PORT dcxPort;
        uint8_t dcxPin;
    };

    class SPI_Device : public DeviceBase
    {
    public:
        SPI_Device(SPI_DeviceDriver *pDriver);

        DEVICE_STATUS Init(SPI_Config *pConfig);

        DEVICE_STATUS ReConfig(SPI_Config *pConfig);

        DEVICE_STATUS Write(uint8_t *writeData, uint32_t dataSize, WaitHandle *waitHandle);

        DEVICE_STATUS WriteRead(uint8_t *writeData, uint32_t dataSize, uint8_t *readBuffer, uint32_t bufferSize,
                                WaitHandle *waitHandle);
    };
}

#endif //STARTUP_SPI_H
