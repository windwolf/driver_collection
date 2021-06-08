#include "basic/command.h"

static void CommandMaster_SendFrameSync(CommandMaster *commandMaster, CommandFrame frame)
{
    COMMAND_BASE_CS_ENABLE(commandMaster->base);
    COMMAND_BASE_DC_SET(commandMaster->base, frame.statusBits.isCmd);
    COMMAND_BASE_RW_SET(commandMaster->base, frame.statusBits.isWrite);

    if (frame.statusBits.isWrite)
    {
        commandMaster->base.device->Tx(commandMaster->base.device, frame.buffer.data, frame.buffer.size, frame.statusBits.dataBits);
    }
    else
    {
        commandMaster->base.device->Rx(commandMaster->base.device, frame.buffer.data, frame.buffer.size, frame.statusBits.dataBits, frame.statusBits.dummyCycles);
    }
    // if (!frame.statusBits.csNotBreak)
    // {
    //     COMMAND_BASE_CS_DISABLE(commandMaster->base);
    // }
};

static void CommandMaster_SendFrameAsync(CommandMaster *commandMaster)
{
    if (commandMaster->_curCommandFrameIndex < commandMaster->_curCommandFrameSize)
    {
        CommandFrame frame = commandMaster->_curCommand[commandMaster->_curCommandFrameIndex];

        COMMAND_BASE_CS_ENABLE(commandMaster->base);
        COMMAND_BASE_DC_SET(commandMaster->base, frame.statusBits.isCmd);
        COMMAND_BASE_RW_SET(commandMaster->base, frame.statusBits.isWrite);

        if (frame.statusBits.isWrite)
        {
            LOG("CMD-SF-A-W:st %lu", commandMaster->_curCommandFrameIndex)
            commandMaster->base.device->TxAsync(commandMaster->base.device, frame.buffer.data, frame.buffer.size, frame.statusBits.dataBits);
            LOG("CMD-SF-A-W:ed %lu", commandMaster->_curCommandFrameIndex)
        }
        else
        {
            LOG("CMD-SF-A-R:st %lu", commandMaster->_curCommandFrameIndex)
            commandMaster->base.device->RxAsync(commandMaster->base.device, frame.buffer.data, frame.buffer.size, frame.statusBits.dataBits, frame.statusBits.dummyCycles);
            LOG("CMD-SF-A-R:ed %lu", commandMaster->_curCommandFrameIndex)
        }
    }
    else
    {
        LOG("CMD-SF-E:")
        COMMAND_BASE_CS_DISABLE(commandMaster->base);
        commandMaster->base.hasError = 0;
        EVENTS_SET_FLAGS(commandMaster->base.events, COMMAND_BASE_EVENT_CMD_COMPLETE);
        EVENTS_RESET_FLAGS(commandMaster->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
    }
};

static inline void CommandMaster_DoTxRxCplt_(PacketIoDevice *device)
{
    CommandMaster *cmd = (CommandMaster *)device->base.host;
    cmd->_curCommandFrameIndex++;
    CommandMaster_SendFrameAsync(cmd);
}

static void CommandMaster_DoError_(PacketIoDevice *device)
{
    CommandMaster *cmd = (CommandMaster *)device->base.host;
    cmd->base.hasError = 1;
    if (CommandBase_IsBusy((CommandBase *)cmd))
    {
        EVENTS_RESET_FLAGS(cmd->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
        EVENTS_SET_FLAGS(cmd->base.events, COMMAND_BASE_EVENT_CMD_COMPLETE);
    }

    if (cmd->base.onError)
    {
        cmd->base.onError((CommandBase *)cmd);
    }
};

DEVICE_STATUS CommandMaster_SendCommandSync(CommandMaster *commandMaster, CommandFrame *command, uint32_t size)
{
    if (!(commandMaster->base.device->opMode & PACKET_IO_DEVICE_OP_MODE_SYNC))
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }
    COMMAND_BASE_RETURN_IF_BUSY(commandMaster->base)

    EVENTS_CLEAR_FLAGS(commandMaster->base.events);

    EVENTS_SET_FLAGS(commandMaster->base.events, COMMAND_BASE_EVENT_CMD_BUSY);

    for (size_t i = 0; i < size; i++)
    {
        CommandMaster_SendFrameSync(commandMaster, command[i]);
    }
    commandMaster->base.hasError = 0;
    EVENTS_SET_FLAGS(commandMaster->base.events, COMMAND_BASE_EVENT_CMD_COMPLETE);
    EVENTS_RESET_FLAGS(commandMaster->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS CommandMaster_SendCommandAsync(CommandMaster *commandMaster, CommandFrame *command, uint32_t size)
{
    if (!(commandMaster->base.device->opMode & PACKET_IO_DEVICE_OP_MODE_ASYNC))
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }

    COMMAND_BASE_RETURN_IF_BUSY(commandMaster->base)

    commandMaster->_curCommand = command;
    commandMaster->_curCommandFrameIndex = 0;
    commandMaster->_curCommandFrameSize = size;

    EVENTS_CLEAR_FLAGS(commandMaster->base.events);

    EVENTS_SET_FLAGS(commandMaster->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
    LOG("CMD-SND-CMD")
    CommandMaster_SendFrameAsync(commandMaster);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS command_master_create(CommandMaster *commandMaster, PacketIoDevice *device,
                                    Pin *csPin, COMMAND_SELECT_PIN_MODE csCfg,
                                    Pin *rwPin, COMMAND_READWRITE_PIN_MODE rwCfg,
                                    Pin *dcPin, COMMAND_DATACMD_PIN_MODE dcCfg)
{
    command_base_create((CommandBase *)commandMaster, device, csPin, csCfg, rwPin, rwCfg, dcPin, dcCfg);
    packet_io_device_host_register(device, commandMaster, &CommandMaster_DoTxRxCplt_, &CommandMaster_DoTxRxCplt_, &CommandMaster_DoError_);
    device->Init(&commandMaster->base.device);
    return DEVICE_STATUS_OK;
};