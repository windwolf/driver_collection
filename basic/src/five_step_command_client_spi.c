#include "../inc/basic/five_step_command_client.h"

static void _send_phase_1(FiveStepCommandClientSpi *commandClient);
static void _send_phase_2(FiveStepCommandClientSpi *commandClient);
static void _send_phase_3(FiveStepCommandClientSpi *commandClient);
static void _send_phase_4(FiveStepCommandClientSpi *commandClient);
static void _send_phase_5(FiveStepCommandClientSpi *commandClient);
static void _send_phase_end(FiveStepCommandClientSpi *commandClient);

static inline BOOL _is_busy(FiveStepCommandClient *commandClient)
{
    ULONG actualFlags;
    return tx_event_flags_get(&commandClient->events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS;
};

static inline void _tx_rx_cplt_(SpiWithPinsDevice *device)
{
    FiveStepCommandClientSpi *cmd = (FiveStepCommandClientSpi *)device->base.parent;
    switch (cmd->_phase)
    {
    case 1:
        return _send_phase_2(cmd);
    case 2:
        return _send_phase_3(cmd);
    case 3:
        return _send_phase_4(cmd);
    case 4:
        return _send_phase_5(cmd);
    case 5:
        return _send_phase_end(cmd);
    }
};

static void _do_error(FiveStepCommandClientSpi *commandClient, DEVICE_STATUS error)
{
    commandClient->base.hasError = 1;
    commandClient->_phase = 0;
    if (_is_busy((FiveStepCommandClient *)commandClient))
    {
        EVENTS_RESET_FLAGS(commandClient->base.events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);
        EVENTS_SET_FLAGS(commandClient->base.events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE);
    }
    if (commandClient->base.onError)
    {
        commandClient->base.onError((FiveStepCommandClient *)commandClient, error);
    }
}

static void _error(DeviceBase *device, DEVICE_STATUS error)
{
    FiveStepCommandClientSpi *cmd = (FiveStepCommandClientSpi *)device->parent;
    _do_error(cmd, error);
};

static void _send_phase_1(FiveStepCommandClientSpi *commandClient)
{
    commandClient->_phase = 1;
    spi_with_pins_device_session_begin(commandClient->device);
    spi_with_pins_device_tx(commandClient->device, 0, &commandClient->base.commandId, 1, commandClient->base.flagBits.commandBits);
};
static void _send_phase_2(FiveStepCommandClientSpi *commandClient)
{
    commandClient->_phase = 2;
    if (commandClient->base.flagBits.hasAddress == 1)
    {
        spi_with_pins_device_tx(commandClient->device, 1, &commandClient->base.address, 1, commandClient->base.flagBits.addressBits);
    }
    else
    {
        _send_phase_3(commandClient);
    }
};
static void _send_phase_3(FiveStepCommandClientSpi *commandClient)
{
    commandClient->_phase = 3;
    if (commandClient->base.flagBits.hasAltData)
    {
        spi_with_pins_device_tx(commandClient->device, 1, &commandClient->base.altData, 1, commandClient->base.flagBits.altDataBits);
    }
    else
    {
        _send_phase_4(commandClient);
    }
};
static void _send_phase_4(FiveStepCommandClientSpi *commandClient)
{
    commandClient->_phase = 4;
    if (commandClient->base.flagBits.dummyCycles != 0)
    {
        _send_phase_end(commandClient);
        commandClient->base.hasError = 1;
        _do_error(commandClient, DEVICE_STATUS_NOT_SUPPORT);
    }
    else
    {
        _send_phase_5(commandClient);
    }
};
static void _send_phase_5(FiveStepCommandClientSpi *commandClient)
{
    commandClient->_phase = 5;
    if (commandClient->base.dataSize > 0)
    {
        if (commandClient->base.flagBits.isWrite)
        {
            spi_with_pins_device_tx(commandClient->device, 1, commandClient->base.data, commandClient->base.dataSize, commandClient->base.flagBits.dataBits);
        }
        else
        {
            spi_with_pins_device_rx(commandClient->device, 1, commandClient->base.data, commandClient->base.dataSize, commandClient->base.flagBits.dataBits, 0);
        }
    }
    else
    {
        _send_phase_end(commandClient);
    }
};
static void _send_phase_end(FiveStepCommandClientSpi *commandClient)
{
    spi_with_pins_device_session_end(commandClient->device);
    commandClient->base.hasError = 0;
    EVENTS_SET_FLAGS(commandClient->base.events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE);
    EVENTS_RESET_FLAGS(commandClient->base.events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);
};

DEVICE_STATUS five_step_command_client_spi_create(FiveStepCommandClientSpi *commandClient, SpiWithPinsDevice *device)
{
    five_step_command_client_create((FiveStepCommandClient *)commandClient);
    commandClient->device = device;
    device->optionBits.autoCs = 0;
    commandClient->_phase = 0;
    _spi_with_pins_device_register(device, commandClient,
                                   &_tx_rx_cplt_,
                                   &_tx_rx_cplt_,
                                   &_error);
    return DEVICE_STATUS_OK;
};

void _five_step_command_client_spi_register(FiveStepCommandClientSpi *commandClient,
                                            FiveStepCommandClientEventHandlerFuncType onError)
{
    commandClient->base.onError = onError;
};

DEVICE_STATUS five_step_command_client_spi_send(FiveStepCommandClientSpi *commandClient)
{
    if (_is_busy((FiveStepCommandClient *)commandClient))
    {
        return DEVICE_STATUS_BUSY;
    }

    EVENTS_CLEAR_FLAGS(commandClient->base.events);

    EVENTS_SET_FLAGS(commandClient->base.events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);

    _send_phase_1(commandClient);

    return DEVICE_STATUS_OK;
};
