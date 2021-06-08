#include "basic/command.h"

static void SimpleCommand_EndPhase3Async(SimpleCommand *simpleCommand);
static void SimpleCommand_SendPhase3Async(SimpleCommand *simpleCommand);

static void SimpleCommand_SendPhase1Async(SimpleCommand *simpleCommand)
{
    simpleCommand->_phase = 1;
    COMMAND_BASE_CS_ENABLE(simpleCommand->base);
    COMMAND_BASE_DC_SET(simpleCommand->base, 1);
    COMMAND_BASE_RW_SET(simpleCommand->base, 1);

    simpleCommand->base.device->TxAsync(simpleCommand->base.device, &simpleCommand->commandId, 1, simpleCommand->flagBits.commandBits);
}

static void SimpleCommand_SendPhase2Async(SimpleCommand *simpleCommand)
{
    simpleCommand->_phase = 2;
    if (simpleCommand->flagBits.hasAddress == 1)
    {
        COMMAND_BASE_DC_SET(simpleCommand->base, 0);
        COMMAND_BASE_RW_SET(simpleCommand->base, 1);
        simpleCommand->base.device->TxAsync(simpleCommand->base.device, &simpleCommand->address, 1, simpleCommand->flagBits.addressBits);
    }
    else
    {
        SimpleCommand_SendPhase3Async(simpleCommand);
    }
};

static void SimpleCommand_SendPhase3Async(SimpleCommand *simpleCommand)
{
    simpleCommand->_phase = 3;
    if (simpleCommand->dataSize > 0)
    {
        COMMAND_BASE_DC_SET(simpleCommand->base, 0);
        if (simpleCommand->flagBits.isWrite)
        {
            COMMAND_BASE_RW_SET(simpleCommand->base, 1);
            simpleCommand->base.device->TxAsync(simpleCommand->base.device, simpleCommand->data, simpleCommand->dataSize, simpleCommand->flagBits.dataBits);
        }
        else
        {
            COMMAND_BASE_RW_SET(simpleCommand->base, 0);
            simpleCommand->base.device->RxAsync(simpleCommand->base.device, simpleCommand->data, simpleCommand->dataSize, simpleCommand->flagBits.dataBits, 0);
        }
    }
    else
    {
        SimpleCommand_EndPhase3Async(simpleCommand);
    }
};

static void SimpleCommand_EndPhase3Async(SimpleCommand *simpleCommand)
{

    COMMAND_BASE_CS_DISABLE(simpleCommand->base);
    simpleCommand->base.hasError = 0;
    EVENTS_SET_FLAGS(simpleCommand->base.events, COMMAND_BASE_EVENT_CMD_COMPLETE);
    EVENTS_RESET_FLAGS(simpleCommand->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
};

static inline void SimpleCommand_DoTxRxCplt_(PacketIoDevice *device)
{
    SimpleCommand *cmd = (SimpleCommand *)device->base.host;
    switch (cmd->_phase)
    {
    case 1:
        return SimpleCommand_SendPhase2Async(cmd);
    case 2:
        return SimpleCommand_SendPhase3Async(cmd);
    case 3:
        return SimpleCommand_EndPhase3Async(cmd);
    }
};

static void SimpleCommand_DoError_(PacketIoDevice *device)
{
    SimpleCommand *cmd = (SimpleCommand *)device->base.host;
    cmd->base.hasError = 1;
    if (CommandBase_IsBusy((CommandBase *)cmd))
    {
        EVENTS_RESET_FLAGS(cmd->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
        EVENTS_SET_FLAGS(cmd->base.events, COMMAND_BASE_EVENT_CMD_COMPLETE);
    }

    if (cmd->base.onError)
    {
        cmd->base.onError(((CommandBase *)cmd));
    }
};

DEVICE_STATUS SimpleCommand_SendCommandSync(SimpleCommand *simpleCommand)
{
    if (!(simpleCommand->base.device->opMode & PACKET_IO_DEVICE_OP_MODE_SYNC))
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }
    COMMAND_BASE_RETURN_IF_BUSY(simpleCommand->base)

    EVENTS_CLEAR_FLAGS(simpleCommand->base.events);

    EVENTS_SET_FLAGS(simpleCommand->base.events, COMMAND_BASE_EVENT_CMD_BUSY);

    COMMAND_BASE_CS_ENABLE(simpleCommand->base);
    COMMAND_BASE_DC_SET(simpleCommand->base, 1);
    COMMAND_BASE_RW_SET(simpleCommand->base, 1);

    simpleCommand->base.device->Tx(simpleCommand->base.device, &simpleCommand->commandId, 1, simpleCommand->flagBits.commandBits);

    if (simpleCommand->flagBits.hasAddress)
    {
        COMMAND_BASE_DC_SET(simpleCommand->base, 0);
        simpleCommand->base.device->Tx(simpleCommand->base.device, &simpleCommand->address, 1, simpleCommand->flagBits.addressBits);
    }

    if (simpleCommand->dataSize > 0)
    {
        COMMAND_BASE_DC_SET(simpleCommand->base, 0);
        if (simpleCommand->flagBits.isWrite)
        {
            COMMAND_BASE_RW_SET(simpleCommand->base, 1);
            simpleCommand->base.device->Tx(simpleCommand->base.device, simpleCommand->data, simpleCommand->dataSize, simpleCommand->flagBits.dataBits);
        }
        else
        {
            COMMAND_BASE_RW_SET(simpleCommand->base, 0);
            simpleCommand->base.device->Rx(simpleCommand->base.device, simpleCommand->data, simpleCommand->dataSize, simpleCommand->flagBits.dataBits, 0);
        }
    }

    COMMAND_BASE_CS_DISABLE(simpleCommand->base);

    simpleCommand->base.hasError = 0;
    EVENTS_SET_FLAGS(simpleCommand->base.events, COMMAND_BASE_EVENT_CMD_COMPLETE);
    EVENTS_RESET_FLAGS(simpleCommand->base.events, COMMAND_BASE_EVENT_CMD_BUSY);
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS SimpleCommand_SendCommandAsync(SimpleCommand *simpleCommand)
{
    if (!(simpleCommand->base.device->opMode & PACKET_IO_DEVICE_OP_MODE_ASYNC))
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }

    COMMAND_BASE_RETURN_IF_BUSY(simpleCommand->base)

    EVENTS_CLEAR_FLAGS(simpleCommand->base.events);

    EVENTS_SET_FLAGS(simpleCommand->base.events, COMMAND_BASE_EVENT_CMD_BUSY);

    SimpleCommand_SendPhase1Async(simpleCommand);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS simple_command_create(SimpleCommand *simpleCommand, PacketIoDevice *device,
                                    Pin *csPin, COMMAND_SELECT_PIN_MODE csCfg,
                                    Pin *rwPin, COMMAND_READWRITE_PIN_MODE rwCfg,
                                    Pin *dcPin, COMMAND_DATACMD_PIN_MODE dcCfg)
{
    command_base_create((CommandBase *)simpleCommand, device, csPin, csCfg, rwPin, rwCfg, dcPin, dcCfg);
    packet_io_device_host_register(device, simpleCommand, &SimpleCommand_DoTxRxCplt_, &SimpleCommand_DoTxRxCplt_, &SimpleCommand_DoError_);
    device->Init(simpleCommand->base.device);
    return DEVICE_STATUS_OK;
};