#ifndef __DEVICE_H__
#define __DEVICE_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"
#include "shared.h"
#include "buffer.h"

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

    typedef void (*DeviceBaseEventHandlerFuncType)(struct DeviceBase *device, OP_RESULT error);
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

    OP_RESULT pin_device_init(PinDevice *device);
    OP_RESULT pin_device_deinit(PinDevice *device);

    OP_RESULT pin_device_set(PinDevice *device, PIN_DEVICE_STATUS parent);
    OP_RESULT pin_device_get(PinDevice *device, PIN_DEVICE_STATUS *parent);
    bool pin_device_is_set(PinDevice *device);
    OP_RESULT pin_device_toggle(PinDevice *device);

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

    OP_RESULT spi_device_init(SpiDevice *device);
    OP_RESULT spi_device_deinit(SpiDevice *device);

    OP_RESULT spi_device_tx(SpiDevice *device, void *data, uint32_t size, DeviceDataWidth width);
    OP_RESULT spi_device_rx(SpiDevice *device, void *data, uint32_t size, DeviceDataWidth width, uint8_t dummyCycleCount);

    /* i2c mem device */
    struct I2CDevice;
    typedef void (*I2CMemDeviceEventHandlerFuncType)(struct I2CDevice *device);
    typedef struct I2CDevice
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
    } I2CDevice;

    void _i2c_device_register(I2CDevice *device, void *parent,
                              I2CMemDeviceEventHandlerFuncType onWriteComplete,
                              I2CMemDeviceEventHandlerFuncType onReadComplete,
                              DeviceBaseEventHandlerFuncType onError);

    OP_RESULT i2c_device_init(I2CDevice *device);
    OP_RESULT i2c_device_deinit(I2CDevice *device);

    OP_RESULT i2c_device_read(I2CDevice *device, uint16_t deviceAddress, void *data, uint32_t size, DeviceDataWidth width);
    OP_RESULT i2c_device_write(I2CDevice *device, uint16_t deviceAddress, void *data, uint32_t size, DeviceDataWidth width);

    OP_RESULT i2c_device_mem_read(I2CDevice *device, uint16_t deviceAddress, uint16_t memAddress, void *data, uint32_t size, DeviceDataWidth width);
    OP_RESULT i2c_device_mem_write(I2CDevice *device, uint16_t deviceAddress, uint16_t memAddress, void *data, uint32_t size, DeviceDataWidth width);

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
        struct
        {
            uint8_t isDmaTx : 1;
            uint8_t isDmaRx : 1;
        } _status;
    } UartDevice;

    void _uart_device_register(UartDevice *device, void *parent,
                               UartDeviceEventHandlerFuncType onTxComplete,
                               UartDeviceRxEventHandlerFuncType onRxComplete,
                               DeviceBaseEventHandlerFuncType onError);

    OP_RESULT uart_device_init(UartDevice *device);
    OP_RESULT uart_device_deinit(UartDevice *device);

    OP_RESULT uart_device_tx(UartDevice *device, uint8_t *data, uint32_t size);
    // DEVICE_STATUS uart_device_rx(UartDevice *device, uint8_t *data, uint32_t size, uint8_t stopOnIdle);

    OP_RESULT uart_device_circular_rx_start(UartDevice *device, uint8_t *data, uint32_t size);
    OP_RESULT uart_device_circular_rx_stop(UartDevice *device);

    struct SdDevice;
    typedef void (*SdDeviceEventHandlerFuncType)(struct SdDevice *device);

    typedef struct SdDevice
    {
        DeviceBase base;
        uint16_t dmaThershold;

        uint32_t cardType;     /*!< Specifies the card Type                         */
        uint32_t cardVersion;  /*!< Specifies the card version                      */
        uint32_t class;        /*!< Specifies the class of the card class           */
        uint32_t relCardAdd;   /*!< Specifies the Relative Card Address             */
        uint32_t blockNbr;     /*!< Specifies the Card Capacity in blocks           */
        uint32_t blockSize;    /*!< Specifies one block size in bytes               */
        uint32_t logBlockNbr;  /*!< Specifies the Card logical Capacity in blocks   */
        uint32_t logBlockSize; /*!< Specifies logical block size in bytes           */
        uint32_t cardSpeed;    /*!< Specifies the card Speed  */

        SdDeviceEventHandlerFuncType onTxComplete;
        SdDeviceEventHandlerFuncType onRxComplete;

        Buffer _rxBuffer;

    } SdDevice;

    void _sd_device_register(SdDevice *device, void *parent,
                             SdDeviceEventHandlerFuncType onTxComplete,
                             SdDeviceEventHandlerFuncType onRxComplete,
                             DeviceBaseEventHandlerFuncType onError);

    OP_RESULT sd_device_init(SdDevice *device);
    OP_RESULT sd_device_deinit(SdDevice *device);
    OP_RESULT sd_device_read(SdDevice *device, void *data, uint32_t num, uint32_t count);
    OP_RESULT sd_device_write(SdDevice *device, void *data, uint32_t num, uint32_t count);
    OP_RESULT sd_device_erase(SdDevice *device, uint32_t num, uint32_t count);

    OP_RESULT sd_device_query_status(SdDevice *device);

    OP_RESULT sd_device_card_init(SdDevice *device);

#ifdef __cplusplus
}
#endif

#endif // __DEVICE_H__