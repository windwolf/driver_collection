#ifndef __BASIC_DEVICE_H__
#define __BASIC_DEVICE_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"
#include "basic/shared.h"
#include "buffer.h"

#define DEVICE_STATUS uint32_t

#define DEVICE_STATUS_OK 0x00000000
#define DEVICE_STATUS_BUSY 0x80000000
#define DEVICE_STATUS_NOT_SUPPORT 0x40000000
#define DEVICE_STATUS_HARDWARE_ERROR 0x20000000
#define DEVICE_STATUS_GENERAL_ERROR 0x10000000

#define ALIGN(n) __attribute__((aligned(n)))

    typedef enum DeviceDataWidth
    {
        DEVICE_DATAWIDTH_8 = 0x00,
        DEVICE_DATAWIDTH_16 = 0x01,
        DEVICE_DATAWIDTH_24 = 0x02,
        DEVICE_DATAWIDTH_32 = 0x03,
    } DeviceDataWidth;

    /* device base */
    struct DeviceBase;

    typedef void (*DeviceBaseEventHandlerFuncType)(struct DeviceBase *device, DEVICE_STATUS error);
    typedef struct DeviceBase
    {
        void *parent;
        void *instance;
        DeviceBaseEventHandlerFuncType onError;
    } DeviceBase;

    void device_base_create(DeviceBase *device);

    void _device_base_register(DeviceBase *device, void *parent,
                               DeviceBaseEventHandlerFuncType onError);
    /* pin device */
    typedef enum PIN_DEVICE_STATUS
    {
        PIN_DEVICE_STATUS_RESET = 0U,
        PIN_DEVICE_STATUS_SET
    } PIN_DEVICE_STATUS;

    typedef enum PIN_DEVICE_STATUS_INVERSE
    {
        PIN_DEVICE_STATUS_INVERSE_NORMAL = 0,
        PIN_DEVICE_STATUS_INVERSE_INVERSE = 1,
    } PIN_DEVICE_STATUS_INVERSE;

    struct PinDevice;

    typedef void (*PinDeviceEventHandlerFuncType)(struct PinDevice *device);

    typedef struct PinDevice
    {
        DeviceBase base;
        uint16_t pinMask;
        PIN_DEVICE_STATUS_INVERSE inverse;
        PinDeviceEventHandlerFuncType onTrigger;
    } PinDevice;

    void _pin_device_register(PinDevice *device, void *parent,
                              PinDeviceEventHandlerFuncType onTrigger,
                              DeviceBaseEventHandlerFuncType onError);

    DEVICE_STATUS pin_device_init(PinDevice *device);
    DEVICE_STATUS pin_device_deinit(PinDevice *device);

    DEVICE_STATUS pin_device_set(PinDevice *device, PIN_DEVICE_STATUS parent);
    DEVICE_STATUS pin_device_get(PinDevice *device, PIN_DEVICE_STATUS *parent);
    DEVICE_STATUS pin_device_toggle(PinDevice *device);

    /* spi device */

    struct SpiDevice;
    typedef void (*SpiDeviceEventHandlerFuncType)(struct SpiDevice *device);

    typedef struct SpiDevice
    {
        DeviceBase base;
        uint16_t dmaThershold;
        SpiDeviceEventHandlerFuncType onTxComplete;
        SpiDeviceEventHandlerFuncType onRxComplete;

        /** @private **/
        Buffer _rxBuffer;
        struct
        {
            uint8_t isDmaTx : 1;
            uint8_t isDmaRx : 1;
        } _status;
    } SpiDevice;

    void _spi_device_register(SpiDevice *device, void *parent,
                              SpiDeviceEventHandlerFuncType onTxComplete,
                              SpiDeviceEventHandlerFuncType onRxComplete,
                              DeviceBaseEventHandlerFuncType onError);

    DEVICE_STATUS spi_device_init(SpiDevice *device);
    DEVICE_STATUS spi_device_deinit(SpiDevice *device);

    DEVICE_STATUS spi_device_tx(SpiDevice *device, void *data, uint32_t size, DeviceDataWidth width);
    DEVICE_STATUS spi_device_rx(SpiDevice *device, void *data, uint32_t size, DeviceDataWidth width, uint8_t dummyCycleCount);

    /* i2c mem device */
    struct I2CMemDevice;
    typedef void (*I2CMemDeviceEventHandlerFuncType)(struct I2CMemDevice *device);
    typedef struct I2CMemDevice
    {
        DeviceBase base;
        uint16_t dmaThershold;

        I2CMemDeviceEventHandlerFuncType onWriteComplete;
        I2CMemDeviceEventHandlerFuncType onReadComplete;

        /** @private **/
        Buffer _rxBuffer;
        struct
        {
            uint8_t isDmaTx : 1;
            uint8_t isDmaRx : 1;
        } _status;
    } I2CMemDevice;

    void _i2c_device_register(I2CMemDevice *device, void *parent,
                              I2CMemDeviceEventHandlerFuncType onWriteComplete,
                              I2CMemDeviceEventHandlerFuncType onReadComplete,
                              DeviceBaseEventHandlerFuncType onError);

    DEVICE_STATUS i2c_device_init(I2CMemDevice *device);
    DEVICE_STATUS i2c_device_deinit(I2CMemDevice *device);

    DEVICE_STATUS i2c_device_read(I2CMemDevice *device, uint16_t deviceAddress, uint16_t memAddress, void *data, uint32_t size, DeviceDataWidth width);
    DEVICE_STATUS i2c_device_write(I2CMemDevice *device, uint16_t deviceAddress, uint16_t memAddress, void *data, uint32_t size, DeviceDataWidth width);

    /* uart device */
    struct UartDevice;
    typedef void (*UartDeviceEventHandlerFuncType)(struct UartDevice *device);
    typedef void (*UartDeviceRxEventHandlerFuncType)(struct UartDevice *device, uint16_t pos);

    typedef struct UartDevice
    {
        DeviceBase base;
        uint16_t dmaThershold;

        UartDeviceEventHandlerFuncType onTxComplete;
        UartDeviceRxEventHandlerFuncType onRxComplete;

        /** @private **/
        Buffer _rxBuffer;
        struct {
            uint8_t isDmaTx : 1;
            uint8_t isDmaRx : 1;
        } _status;
    } UartDevice;

    void _uart_device_register(UartDevice *device, void *parent,
                               UartDeviceEventHandlerFuncType onTxComplete,
                               UartDeviceRxEventHandlerFuncType onRxComplete,
                               DeviceBaseEventHandlerFuncType onError);

    DEVICE_STATUS uart_device_init(UartDevice *device);
    DEVICE_STATUS uart_device_deinit(UartDevice *device);

    DEVICE_STATUS uart_device_tx(UartDevice *device, uint8_t *data, uint32_t size);
    //DEVICE_STATUS uart_device_rx(UartDevice *device, uint8_t *data, uint32_t size, uint8_t stopOnIdle);

    DEVICE_STATUS uart_device_circular_rx_start(UartDevice *device, uint8_t *data, uint32_t size);
    DEVICE_STATUS uart_device_circular_rx_stop(UartDevice *device);

#ifdef __cplusplus
}
#endif

#endif // __BASIC_DEVICE_H__