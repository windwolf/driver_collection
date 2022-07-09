#include "bsp_i2c.h"
#include "bsp_shared.h"
#include "os/os.h"

#ifdef HAL_GPIO_MODULE_ENABLED

#define SCL_SET(device) (pin_device_set(device->scl, PIN_DEVICE_STATUS_SET))
#define SCL_RESET(device) (pin_device_set(device->scl, PIN_DEVICE_STATUS_RESET))
#define SDA_SET(device) (pin_device_set(device->sda, PIN_DEVICE_STATUS_SET))
#define SDA_RESET(device) (pin_device_set(device->sda, PIN_DEVICE_STATUS_RESET))
#define SDA_IS_SET(device) (pin_device_is_set(device->sda))
#define UNWRAP_DEVICE(device) ((SoftI2CDevice *)(device->base.instance))

static void _softi2c_start(SoftI2CDevice *device);
static void _softi2c_stop(SoftI2CDevice *device);
static bool _softi2c_waitAck(SoftI2CDevice *device, uint32_t timeout);
static void _softi2c_writeByte(SoftI2CDevice *device, uint8_t byte);

OP_RESULT i2c_device_create(I2CDevice *device, SoftI2CDevice *instance)
{
    device_base_create((DeviceBase *)device);
    device->base.instance = instance;

    device->onReadComplete = NULL;
    device->onWriteComplete = NULL;
    return OP_RESULT_OK;
};

OP_RESULT i2c_device_init(I2CDevice *device) { return OP_RESULT_OK; };
OP_RESULT i2c_device_deinit(I2CDevice *device) { return OP_RESULT_OK; };

OP_RESULT i2c_device_read(I2CDevice *device, uint16_t deviceAddress, void *data, uint32_t size, DeviceDataWidth width)
{
    return OP_RESULT_NOT_SUPPORT;
};

OP_RESULT i2c_device_write(I2CDevice *device, uint16_t deviceAddress, void *data, uint32_t size, DeviceDataWidth width)
{
    SoftI2CDevice *dev = UNWRAP_DEVICE(device);
    _softi2c_start(dev);
    _softi2c_writeByte(dev, (uint8_t)deviceAddress & 0xFE); // Slave address,SA0=0
    if (!_softi2c_waitAck(dev, 20))
    {
        return OP_RESULT_TIMEOUT;
    }
    for (uint32_t i = 0; i < size; i++)
    {
        _softi2c_writeByte(dev, ((uint8_t *)data)[i]);
        if (!_softi2c_waitAck(dev, 20))
        {
            return OP_RESULT_TIMEOUT;
        }
    }
    _softi2c_stop(dev);
    return OP_RESULT_OK;
};

OP_RESULT i2c_device_mem_write(I2CDevice *device, uint16_t deviceAddress, uint16_t memAddress, void *data, uint32_t size, DeviceDataWidth width)
{
    SoftI2CDevice *dev = UNWRAP_DEVICE(device);
    _softi2c_start(dev);
    _softi2c_writeByte(dev, (uint8_t)deviceAddress & 0xFE); // Slave address,SA0=0
    if (!_softi2c_waitAck(dev, 20))
    {
        return OP_RESULT_TIMEOUT;
    }

    if (width == DEVICE_DATAWIDTH_8)
    {
        _softi2c_writeByte(dev, (uint8_t)memAddress);
        if (!_softi2c_waitAck(dev, 20))
        {
            return OP_RESULT_TIMEOUT;
        }
    }
    else if (width == DEVICE_DATAWIDTH_16)
    {
        _softi2c_writeByte(dev, (uint8_t)(memAddress >> 8));
        if (!_softi2c_waitAck(dev, 20))
        {
            return OP_RESULT_TIMEOUT;
        }
        _softi2c_writeByte(dev, (uint8_t)memAddress);
        if (!_softi2c_waitAck(dev, 20))
        {
            return OP_RESULT_TIMEOUT;
        }
    }
    else
    {
        return OP_RESULT_NOT_SUPPORT;
    }

    for (uint32_t i = 0; i < size; i++)
    {
        _softi2c_writeByte(dev, ((uint8_t *)data)[i]);
        if (!_softi2c_waitAck(dev, 20))
        {
            return OP_RESULT_TIMEOUT;
        }
    }
    _softi2c_stop(dev);
    return OP_RESULT_OK;
};

OP_RESULT i2c_device_mem_read(I2CDevice *device, uint16_t deviceAddress, uint16_t memAddress, void *data, uint32_t size, DeviceDataWidth width)
{
    return OP_RESULT_NOT_SUPPORT;
};

static void _softi2c_start(SoftI2CDevice *device)
{
    pin_device_mode_set(device->sda, PIN_DEVICE_MODE_OUTPUT);
    pin_device_mode_set(device->scl, PIN_DEVICE_MODE_OUTPUT);
    SDA_SET(device);
    SCL_SET(device);
    bsp_delay_us(4);
    SDA_RESET(device);
    bsp_delay_us(4);
    SCL_RESET(device); // hold the bus
    bsp_delay_us(4);
}

static void _softi2c_stop(SoftI2CDevice *device)
{
    SCL_RESET(device);
    bsp_delay_us(4);
    SDA_RESET(device);
    bsp_delay_us(4);
    SCL_SET(device);
    bsp_delay_us(4);
    SDA_SET(device);
    bsp_delay_us(4);
}

static bool _softi2c_waitAck(SoftI2CDevice *device, uint32_t timeout)
{
    bool ack = false;

    SCL_RESET(device);
    bsp_delay_us(2);
    pin_device_mode_set(device->sda, PIN_DEVICE_MODE_INPUT);
    bsp_delay_us(2);
    SCL_SET(device);
    bsp_delay_us(4);
    for (uint8_t i = 20; i > 0; i--)
    {
        if (!(SDA_IS_SET(device)))
        {
            ack = true;
            break;
        }
    }
    pin_device_mode_set(device->sda, PIN_DEVICE_MODE_OUTPUT);
    return ack;
}

static void _softi2c_writeByte(SoftI2CDevice *device, uint8_t byte)
{
    uint8_t i = 8;
    while (i--)
    {
        SCL_RESET(device);
        bsp_delay_us(4);
        if (byte & 0x80)
        {
            SDA_SET(device);
        }
        else
        {
            SDA_RESET(device);
        }
        bsp_delay_us(4);
        byte <<= 1;
        SCL_SET(device);
        bsp_delay_us(4);
    }
}

#endif // HAL_GPIO_MODULE_ENABLED