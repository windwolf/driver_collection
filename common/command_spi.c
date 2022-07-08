#include "common/command.h"

static void _send_phase_1(CommandSpi *command);
static void _send_phase_2(CommandSpi *command);
static void _send_phase_3(CommandSpi *command);
static void _send_phase_4(CommandSpi *command);
static void _send_phase_5(CommandSpi *command);
static void _send_phase_end(CommandSpi *command);

static inline void _tx_rx_cplt_(SpiWithPinsDevice *device)
{
    CommandSpi *cmd = (CommandSpi *)device->base.parent;
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

static void _do_error(CommandSpi *command, OP_RESULT error)
{
    command->base.hasError = 1;
    command->_phase = 0;
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

static void _send_phase_1(CommandSpi *command)
{
    command->_phase = 1;
    if (command->base._curFrame->commandMode != COMMAND_FRAME_MODE_SKIP)
    {
        spi_with_pins_device_session_begin(command->device);
        spi_with_pins_device_tx(command->device, 0, &command->base._curFrame->commandId, 1, DEVICE_DATAWIDTH_8);
    }
    else
    {
        _send_phase_2(command);
    }
};
static void _send_phase_2(CommandSpi *command)
{
    command->_phase = 2;
    if (command->base._curFrame->addressMode != COMMAND_FRAME_MODE_SKIP)
    {
        spi_with_pins_device_tx(command->device, 1, &command->base._curFrame->address, 1, command->base._curFrame->addressBits);
    }
    else
    {
        _send_phase_3(command);
    }
};
static void _send_phase_3(CommandSpi *command)
{
    command->_phase = 3;
    if (command->base._curFrame->altDataMode != COMMAND_FRAME_MODE_SKIP)
    {
        spi_with_pins_device_tx(command->device, 1, &command->base._curFrame->altData, 1, command->base._curFrame->altDataBits);
    }
    else
    {
        _send_phase_4(command);
    }
};
static void _send_phase_4(CommandSpi *command)
{
    command->_phase = 4;
    if (command->base._curFrame->dummyCycles != 0)
    {
        _send_phase_end(command);
        command->base.hasError = 1;
        _do_error(command, OP_RESULT_NOT_SUPPORT);
    }
    else
    {
        _send_phase_5(command);
    }
};
static void _send_phase_5(CommandSpi *command)
{
    command->_phase = 5;
    if (command->base._curFrame->dataSize > 0 && command->base._curFrame->dataMode != COMMAND_FRAME_MODE_SKIP)
    {
        if (command->base._curFrame->isWrite)
        {
            spi_with_pins_device_tx(command->device, 1, command->base._curFrame->data, command->base._curFrame->dataSize, command->base._curFrame->dataBits);
        }
        else
        {
            spi_with_pins_device_rx(command->device, 1, command->base._curFrame->data, command->base._curFrame->dataSize, command->base._curFrame->dataBits, 0);
        }
    }
    else
    {
        _send_phase_end(command);
    }
};
static void _send_phase_end(CommandSpi *command)
{
    spi_with_pins_device_session_end(command->device);
    command->base.hasError = 0;
    command_end(&(command->base));
};

static OP_RESULT _five_step_command_client_spi_send(Command *command, CommandFrame *commandStep)
{
    if (command_is_busy(command))
    {
        return OP_RESULT_BUSY;
    }

    command_start(command);
    command->_curFrame = commandStep;
    _send_phase_1((CommandSpi *)command);

    return OP_RESULT_OK;
};

OP_RESULT command_spi_create(CommandSpi *command, SpiWithPinsDevice *device)
{
    command_create((Command *)command, &_five_step_command_client_spi_send);
    device->optionBits.autoCs = 0;
    command->device = device;
    command->_phase = 0;
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
