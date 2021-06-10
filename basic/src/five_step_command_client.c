#include "../inc/basic/five_step_command_client.h"

static void _send_phase_1(FiveStepCommandCient *commandClient);
static void _send_phase_2(FiveStepCommandCient *commandClient);
static void _send_phase_3(FiveStepCommandCient *commandClient);
static void _send_phase_4(FiveStepCommandCient *commandClient);
static void _send_phase_5(FiveStepCommandCient *commandClient);
static void _send_phase_end(FiveStepCommandCient *commandClient);

static inline BOOL _is_busy(FiveStepCommandCient *commandClient)
{
    ULONG actualFlags;
    return tx_event_flags_get(&commandClient->events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS;
};

static inline void _tx_rx_cplt_(SpiWithPinsDevice *device)
{
    FiveStepCommandCient *cmd = (FiveStepCommandCient *)device->base.parent;
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

static void _do_error(FiveStepCommandCient *commandClient, DEVICE_STATUS error)
{
    commandClient->hasError = 1;
    commandClient->_phase = 0;
    if (_is_busy(commandClient))
    {
        EVENTS_RESET_FLAGS(commandClient->events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);
        EVENTS_SET_FLAGS(commandClient->events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE);
    }
    if (commandClient->onError)
    {
        commandClient->onError(commandClient, error);
    }
}

static void _error(SpiWithPinsDevice *device, DEVICE_STATUS error)
{
    FiveStepCommandCient *cmd = (FiveStepCommandCient *)device->base.parent;
    _do_error(cmd, error);
};

static void _send_phase_1(FiveStepCommandCient *commandClient)
{
    commandClient->_phase = 1;
    spi_with_pins_device_session_begin(commandClient->device);
    spi_with_pins_device_tx(commandClient->device, 0, &commandClient->commandId, 1, commandClient->flagBits.commandBits);
};
static void _send_phase_2(FiveStepCommandCient *commandClient)
{
    commandClient->_phase = 2;
    if (commandClient->flagBits.hasAddress == 1)
    {
        spi_with_pins_device_tx(commandClient->device, 1, &commandClient->address, 1, commandClient->flagBits.addressBits);
    }
    else
    {
        _send_phase_3(commandClient);
    }
};
static void _send_phase_3(FiveStepCommandCient *commandClient)
{
    commandClient->_phase = 3;
    if (commandClient->altDataSize > 0)
    {
        spi_with_pins_device_tx(commandClient->device, 1, &commandClient->altData, commandClient->altDataSize, commandClient->flagBits.altDataBits);
    }
    else
    {
        _send_phase_4(commandClient);
    }
};
static void _send_phase_4(FiveStepCommandCient *commandClient)
{
    commandClient->_phase = 4;
    if (commandClient->flagBits.dummyCycles != 0)
    {
        _send_phase_end(commandClient);
        commandClient->hasError = 1;
        _do_error(commandClient, DEVICE_STATUS_NOT_SUPPORT);
    }
    else
    {
        _send_phase_5(commandClient);
    }
};
static void _send_phase_5(FiveStepCommandCient *commandClient)
{
    commandClient->_phase = 5;
    if (commandClient->dataSize > 0)
    {
        if (commandClient->flagBits.isWrite)
        {
            spi_with_pins_device_tx(commandClient->device, 1, &commandClient->data, commandClient->dataSize, commandClient->flagBits.dataBits);
        }
        else
        {
            spi_with_pins_device_rx(commandClient->device, 1, &commandClient->data, commandClient->dataSize, commandClient->flagBits.dataBits, 0);
        }
    }
    else
    {
        _send_phase_end(commandClient);
    }
};
static void _send_phase_end(FiveStepCommandCient *commandClient)
{
    spi_with_pins_device_session_end(commandClient->device);
    commandClient->hasError = 0;
    EVENTS_SET_FLAGS(commandClient->events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE);
    EVENTS_RESET_FLAGS(commandClient->events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);
};

DEVICE_STATUS five_step_command_client_create(FiveStepCommandCient *commandClient, SpiWithPinsDevice *device)
{
    commandClient->device = device;
    device->optionBits.autoCs = 0;
    tx_event_flags_create(&(commandClient->events), "5stepcmd");
    commandClient->hasError = 0;
    commandClient->_phase = 0;
    commandClient->onError = NULL;
    _spi_with_pins_device_register(device, commandClient,
                                   &_tx_rx_cplt_,
                                   &_tx_rx_cplt_,
                                   &_error);
};

void _five_step_command_client_register(FiveStepCommandCient *commandClient,
                                        FiveStepCommandCientEventHandlerFuncType onError)
{
    commandClient->onError = onError;
};

DEVICE_STATUS five_step_command_client_cplt_wait(FiveStepCommandCient *commandClient, ULONG timeout)
{
    ULONG actualFlags;
    tx_event_flags_get(&commandClient->events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE, TX_AND_CLEAR, &actualFlags, TX_WAIT_FOREVER);
    return commandClient->hasError ? DEVICE_STATUS_GENERAL_ERROR : DEVICE_STATUS_OK;
};

DEVICE_STATUS five_step_command_client_send(FiveStepCommandCient *commandClient)
{
    if (_is_busy(commandClient))
    {
        return DEVICE_STATUS_BUSY;
    }

    EVENTS_CLEAR_FLAGS(commandClient->events);

    EVENTS_SET_FLAGS(commandClient->events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);

    _send_phase_1(commandClient);

    return DEVICE_STATUS_OK;
};
