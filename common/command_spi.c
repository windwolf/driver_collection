#include "common/command.h"

#define COMMAND_STEP_DONE 0x02UL

static inline void _tx_rx_cplt_(SpiWithPinsDevice *device)
{
    CommandSpi *cmd = (CommandSpi *)device->base.parent;
    ww_os_events_set(&(cmd->base.events), COMMAND_STEP_DONE);
};

static void _do_error(CommandSpi *command, OP_RESULT error)
{
    command->base.hasError = 1;
    if (command_is_busy(&(command->base)))
    {
        command_end(&(command->base));
    }
    if (command->base.onError)
    {
        command->base.onError((Command *)command, error);
    }
}

static void _error(DeviceBase *device, OP_RESULT error)
{
    CommandSpi *cmd = (CommandSpi *)device->parent;
    _do_error(cmd, error);
};

static OP_RESULT _five_step_command_client_spi_send(Command *command, CommandFrame *commandStep)
{
    if (command_is_busy(command))
    {
        return OP_RESULT_BUSY;
    }

    CommandSpi *cmdSpi = (CommandSpi *)command;
    command_start(command);
    ww_os_events_reset(&(command->events), COMMAND_STEP_DONE);
    spi_with_pins_device_session_begin(cmdSpi->device);
    if (commandStep->commandMode != COMMAND_FRAME_MODE_SKIP)
    {

        spi_with_pins_device_tx(cmdSpi->device, 0, &commandStep->commandId, 1, DEVICE_DATAWIDTH_8);
        ww_os_events_get(&(command->events), COMMAND_STEP_DONE, DRIVER_EVENTS_OPTION_AND, command->timeout);
        ww_os_events_reset(&(command->events), COMMAND_STEP_DONE);
    }
    if (commandStep->addressMode != COMMAND_FRAME_MODE_SKIP)
    {
        spi_with_pins_device_tx(cmdSpi->device, 1, &commandStep->address, 1, commandStep->addressBits);
        ww_os_events_get(&(command->events), COMMAND_STEP_DONE, DRIVER_EVENTS_OPTION_AND, command->timeout);
        ww_os_events_reset(&(command->events), COMMAND_STEP_DONE);
    }
    if (commandStep->altDataMode != COMMAND_FRAME_MODE_SKIP)
    {
        spi_with_pins_device_tx(cmdSpi->device, 1, &commandStep->altData, 1, commandStep->altDataBits);
        ww_os_events_get(&(command->events), COMMAND_STEP_DONE, DRIVER_EVENTS_OPTION_AND, command->timeout);
        ww_os_events_reset(&(command->events), COMMAND_STEP_DONE);
    }
    if (commandStep->dummyCycles != 0)
    {
        spi_with_pins_device_session_end(cmdSpi->device);
        command->hasError = 1;
        _do_error(cmdSpi, OP_RESULT_NOT_SUPPORT);
        command_end(command);
    }
    else if (commandStep->dataSize > 0 && commandStep->dataMode != COMMAND_FRAME_MODE_SKIP)
    {
        if (commandStep->isWrite)
        {
            spi_with_pins_device_tx(cmdSpi->device, 1, commandStep->data, commandStep->dataSize, commandStep->dataBits);
        }
        else
        {
            spi_with_pins_device_rx(cmdSpi->device, 1, commandStep->data, commandStep->dataSize, commandStep->dataBits, 0);
        }
        ww_os_events_get(&(command->events), COMMAND_STEP_DONE, DRIVER_EVENTS_OPTION_AND, command->timeout);
        ww_os_events_reset(&(command->events), COMMAND_STEP_DONE);
    }
    spi_with_pins_device_session_end(cmdSpi->device);
    command->hasError = 0;
    command_end(command);
    return OP_RESULT_OK;
};

OP_RESULT command_spi_create(CommandSpi *command, SpiWithPinsDevice *device)
{
    command_create((Command *)command, &_five_step_command_client_spi_send);
    device->optionBits.autoCs = 0;
    command->device = device;
    _spi_with_pins_device_register(device, command,
                                   &_tx_rx_cplt_,
                                   &_tx_rx_cplt_,
                                   &_error);
    return OP_RESULT_OK;
};

void _command_spi_register(CommandSpi *command,
                           CommandErrorHandleFuncType onError)
{
    command->base.onError = onError;
};
