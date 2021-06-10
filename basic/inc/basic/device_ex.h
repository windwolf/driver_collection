#ifndef __BASIC_DEVICE_EX_H__
#define __BASIC_DEVICE_EX_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "device.h"
    /* spi with pins device */

    typedef enum SPI_WITH_PINS_DEVICE_SELECT_PIN_MODE
    {
        SPI_WITH_PINS_DEVICE_SELECT_PIN_MODE_SELECT = 0x00,
        SPI_WITH_PINS_DEVICE_SELECT_PIN_MODE_UNSELECT = 0x01,
    } SPI_WITH_PINS_DEVICE_SELECT_PIN_MODE;

    typedef enum SPI_WITH_PINS_DEVICE_DATACMD_PIN_MODE
    {
        SPI_WITH_PINS_DEVICE_DATACMD_PIN_MODE_CMD = 0x00,
        SPI_WITH_PINS_DEVICE_DATACMD_PIN_MODE_DATA = 0x01,

    } SPI_WITH_PINS_DEVICE_DATACMD_PIN_MODE;

    typedef enum SPI_WITH_PINS_DEVICE_READWRITE_PIN_MODE
    {
        SPI_WITH_PINS_DEVICE_READWRITE_PIN_MODE_WRITE = 0x00,
        SPI_WITH_PINS_DEVICE_READWRITE_PIN_MODE_READ = 0x01,

    } SPI_WITH_PINS_DEVICE_READWRITE_PIN_MODE;

    struct SpiWithPinsDevice;
    typedef void (*SpiWithPinsDeviceEventHandlerFuncType)(struct SpiWithPinsDevice *device);

    typedef struct SpiWithPinsDevice
    {
        DeviceBase base;
        SpiDevice *spi;
        PinDevice *csPin;
        PinDevice *readPin;
        PinDevice *dataPin;
        union
        {
            uint8_t options;
            struct
            {
                uint8_t autoCs : 1;
            } optionBits;
        };
        SpiWithPinsDeviceEventHandlerFuncType onTxComplete;
        SpiWithPinsDeviceEventHandlerFuncType onRxComplete;

        uint8_t _isSessionBusy;
    } SpiWithPinsDevice;

    DEVICE_STATUS spi_with_pins_device_create(SpiWithPinsDevice *device, SpiDevice *spi,
                                              PinDevice *csPin,
                                              PinDevice *readPin,
                                              PinDevice *dataPin);

    void _spi_with_pins_device_register(SpiWithPinsDevice *device, void *parent,
                                        SpiWithPinsDeviceEventHandlerFuncType onTxComplete,
                                        SpiWithPinsDeviceEventHandlerFuncType onRxComplete,
                                        DeviceBaseEventHandlerFuncType onError);

    DEVICE_STATUS spi_with_pins_device_init(SpiWithPinsDevice *device);
    DEVICE_STATUS spi_with_pins_device_deinit(SpiWithPinsDevice *device);

    DEVICE_STATUS spi_with_pins_device_tx(SpiWithPinsDevice *device, uint8_t isData, void *data, uint32_t size, DeviceDataWidth width);
    DEVICE_STATUS spi_with_pins_device_rx(SpiWithPinsDevice *device, uint8_t isData, void *data, uint32_t size, DeviceDataWidth width, uint8_t dummyCycleCount);

    DEVICE_STATUS spi_with_pins_device_session_begin(SpiWithPinsDevice *device);
    DEVICE_STATUS spi_with_pins_device_session_end(SpiWithPinsDevice *device);

#ifdef __cplusplus
}
#endif

#endif // __BASIC_DEVICE_EX_H__