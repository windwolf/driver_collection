#include "../inc/basic/device_ex.h"

static inline void cs_enable(PinDevice *csPin)
{
    if (csPin != NULL)
        pin_device_set(csPin, PIN_DEVICE_STATUS_SET);
};

static inline void cs_disable(PinDevice *csPin)
{
    if (csPin != NULL)
        pin_device_set(csPin, PIN_DEVICE_STATUS_RESET);
};

static inline void dc_set(PinDevice *dcPin, uint8_t isData)
{
    if (dcPin != NULL)
        pin_device_set(dcPin, isData);
};

static inline void rw_set(PinDevice *rwPin, uint8_t isRead)
{
    if (rwPin != NULL)
        pin_device_set(rwPin, isRead);
};

static void _spi_tx_clpt(struct SpiDevice *device)
{
    SpiWithPinsDevice *spDev = (SpiWithPinsDevice *)device->base.parent;
    if (spDev->optionBits.autoCs || spDev->_isSessionBusy)
    {
        cs_disable(spDev->csPin);
    }
    if (spDev->onTxComplete != NULL)
    {
        spDev->onTxComplete(spDev);
    }
};

static void _spi_rx_clpt(struct SpiDevice *device)
{
    SpiWithPinsDevice *spDev = (SpiWithPinsDevice *)device->base.parent;
    if (spDev->optionBits.autoCs || spDev->_isSessionBusy)
    {
        cs_disable(spDev->csPin);
    }

    if (spDev->onRxComplete != NULL)
    {
        spDev->onRxComplete(spDev);
    }
};

static void _spi_err(struct SpiDevice *device, DEVICE_STATUS error)
{
    SpiWithPinsDevice *spDev = (SpiWithPinsDevice *)device->base.parent;
    cs_disable(spDev->csPin);
    spDev->_isSessionBusy = 0;

    if (spDev->base.onError != NULL)
    {
        spDev->base.onError(spDev, error);
    }
};

DEVICE_STATUS spi_with_pins_device_create(SpiWithPinsDevice *device, SpiDevice *spi,
                                          PinDevice *csPin,
                                          PinDevice *readPin,
                                          PinDevice *dataPin)
{
    device_base_create((DeviceBase *)device);
    device->spi = spi;
    device->csPin = csPin;
    device->readPin = readPin;
    device->dataPin = dataPin;
    device->_isSessionBusy = 0;
    device->onTxComplete = NULL;
    device->onRxComplete = NULL;
    _spi_device_register(spi, device, &_spi_tx_clpt, &_spi_rx_clpt, &_spi_err);
    return DEVICE_STATUS_OK;
};

void _spi_with_pins_device_register(SpiWithPinsDevice *device, void *parent,
                                    SpiWithPinsDeviceEventHandlerFuncType onTxComplete,
                                    SpiWithPinsDeviceEventHandlerFuncType onRxComplete,
                                    DeviceBaseEventHandlerFuncType onError)
{
    _device_base_register(device, parent, onError);
    device->onTxComplete = onTxComplete;
    device->onRxComplete = onRxComplete;
};

DEVICE_STATUS spi_with_pins_device_init(SpiWithPinsDevice *device)
{
    return spi_device_init(device->spi);
};

DEVICE_STATUS spi_with_pins_device_deinit(SpiWithPinsDevice *device)
{
    return spi_device_deinit(device->spi);
};

DEVICE_STATUS spi_with_pins_device_tx(SpiWithPinsDevice *device, uint8_t isData, void *data, uint32_t size, DeviceDataWidth width)
{
    if (device->optionBits.autoCs || device->_isSessionBusy)
    {
        cs_enable(device->csPin);
    }

    rw_set(device->readPin, 0);
    dc_set(device->dataPin, (uint32_t)isData);
    return spi_device_tx(device->spi, data, size, width);
};
DEVICE_STATUS spi_with_pins_device_rx(SpiWithPinsDevice *device, uint8_t isData, void *data, uint32_t size, DeviceDataWidth width, uint8_t dummyCycleCount)
{
    if (device->optionBits.autoCs || device->_isSessionBusy)
    {
        cs_enable(device->csPin);
    }
    rw_set(device->readPin, 1);
    dc_set(device->dataPin, (uint32_t)isData);
    return spi_device_rx(device->spi, data, size, width, dummyCycleCount);
};

DEVICE_STATUS spi_with_pins_device_session_begin(SpiWithPinsDevice *device)
{
    if (device->_isSessionBusy)
    {
        return DEVICE_STATUS_BUSY;
    }
    device->_isSessionBusy = 1;
    return DEVICE_STATUS_OK;
};
DEVICE_STATUS spi_with_pins_device_session_end(SpiWithPinsDevice *device)
{
    device->_isSessionBusy = 0;
    cs_disable(device->csPin);
    return DEVICE_STATUS_OK;
};
