#include "../inc/basic/command_client.h"

static void _command_client_send_frame(CommandClient *commandClient);

static BOOL _command_client_is_busy(CommandClient *commandClient)
{
    ULONG actualFlags;
    return tx_event_flags_get(&commandClient->events, COMMAND_CLIENT_EVENT_CMD_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS;
};

static inline void _command_client_tx_rx_cplt(SpiWithPinsDevice *device)
{
    CommandClient *cmd = (CommandClient *)device->base.parent;
    cmd->_curCommandFrameIndex++;
    _command_client_send_frame(cmd);
};

static void _command_client_error(DeviceBase *device, DEVICE_STATUS error)
{
    CommandClient *cmd = (CommandClient *)device->parent;
    cmd->hasError = 1;
    if (_command_client_is_busy(cmd))
    {
        EVENTS_RESET_FLAGS(cmd->events, COMMAND_CLIENT_EVENT_CMD_BUSY);
        EVENTS_SET_FLAGS(cmd->events, COMMAND_CLIENT_EVENT_CMD_COMPLETE);
    }

    if (cmd->onError)
    {
        cmd->onError((CommandClient *)cmd, error);
    }
};

static void _command_client_send_frame(CommandClient *commandClient)
{
    if (commandClient->_curCommandFrameIndex < commandClient->_curCommandFrameSize)
    {
        CommandFrame frame = commandClient->_curCommandFrame[commandClient->_curCommandFrameIndex];

        if (frame.statusBits.isRead)
        {
            LOG("CMD-SF-A-W:st %u", commandClient->_curCommandFrameIndex)
            spi_with_pins_device_tx(commandClient->device, frame.statusBits.isData, frame.buffer.data, frame.buffer.size, frame.statusBits.dataBits);
            LOG("CMD-SF-A-W:ed %u", commandClient->_curCommandFrameIndex)
        }
        else
        {
            LOG("CMD-SF-A-R:st %u", commandClient->_curCommandFrameIndex)
            spi_with_pins_device_rx(commandClient->device, frame.statusBits.isData, frame.buffer.data, frame.buffer.size, frame.statusBits.dataBits, frame.statusBits.dummyCycles);
            LOG("CMD-SF-A-R:ed %u", commandClient->_curCommandFrameIndex)
        }
    }
    else
    {
        LOG("CMD-SF-E:")
        commandClient->hasError = 0;
        EVENTS_SET_FLAGS(commandClient->events, COMMAND_CLIENT_EVENT_CMD_COMPLETE);
        EVENTS_RESET_FLAGS(commandClient->events, COMMAND_CLIENT_EVENT_CMD_BUSY);
    }
};

DEVICE_STATUS command_client_create(CommandClient *commandClient, SpiWithPinsDevice *device)
{
    commandClient->device = device;
    tx_event_flags_create(&(commandClient->events), "command");
    commandClient->hasError = 0;
    commandClient->_curCommandFrame = NULL;
    commandClient->_curCommandFrameSize = 0;
    commandClient->_curCommandFrameIndex = -1;
    commandClient->onError = NULL;

    _spi_with_pins_device_register(device, commandClient,
                                   &_command_client_tx_rx_cplt,
                                   &_command_client_tx_rx_cplt,
                                   &_command_client_error);
    return DEVICE_STATUS_OK;
};

void _command_client_register(CommandClient *commandClient, CommandClientEventHandlerFuncType onError)
{
    commandClient->onError = onError;
};

DEVICE_STATUS command_client_send(CommandClient *commandClient, CommandFrame *commandFrame, uint32_t size)
{
    if (_command_client_is_busy(commandClient))
    {
        return DEVICE_STATUS_BUSY;
    }

    commandClient->_curCommandFrame = commandFrame;
    commandClient->_curCommandFrameIndex = 0;
    commandClient->_curCommandFrameSize = size;

    EVENTS_CLEAR_FLAGS(commandClient->events);

    EVENTS_SET_FLAGS(commandClient->events, COMMAND_CLIENT_EVENT_CMD_BUSY);
    LOG("CMD-SND-CMD")
    _command_client_send_frame(commandClient);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS command_client_cplt_wait(CommandClient *commandClient, ULONG timeout)
{
    ULONG actualFlags;
    tx_event_flags_get(&commandClient->events, COMMAND_CLIENT_EVENT_CMD_COMPLETE, TX_AND_CLEAR, &actualFlags, TX_WAIT_FOREVER);
    return commandClient->hasError ? DEVICE_STATUS_GENERAL_ERROR : DEVICE_STATUS_OK;
};
