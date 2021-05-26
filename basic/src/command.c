#include "basic/command.h"
#include "basic/shared.h"

#define COMMAND_MASTER_EVENT_TX_BUSY 0x01
#define COMMAND_MASTER_EVENT_TX_COMPLETE 0x02

#define COMMAND_MASTER_EVENT_RX_BUSY 0x04
#define COMMAND_MASTER_EVENT_RX_COMPLETE 0x08

#define COMMAND_MASTER_EVENT_CMD_BUSY 0x10
#define COMMAND_MASTER_EVENT_CMD_COMPLETE 0x20

#define COMMAND_MASTER_CS_ENABLE(cm) \
    if (cm->csPin != NULL)           \
    Pin_Set(cm->csPin, !cm->csPinCfg)
#define COMMAND_MASTER_CS_DISABLE(cm) \
    if (cm->csPin != NULL)            \
    Pin_Set(cm->csPin, cm->csPinCfg)
#define COMMAND_MASTER_DC_SET(cm, isCmd) \
    if (cm->dcPin != NULL)               \
    Pin_Set(cm->dcPin, (isCmd) ^ (cm->dcPinCfg))
#define COMMAND_MASTER_RW_SET(cm, isWrite) \
    if (cm->rwPin != NULL)                 \
    Pin_Set(cm->rwPin, (isWrite) ^ (cm->rwPinCfg))

static inline int CommandMaster_IsBusy(CommandMaster *commandMaster)
{
    ULONG actualFlags;
    return tx_event_flags_get(&commandMaster->events, COMMAND_MASTER_EVENT_CMD_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS;
}

static void CommandMaster_SendFrameSync(CommandMaster *commandMaster, CommandFrame frame)
{
    COMMAND_MASTER_CS_ENABLE(commandMaster);
    COMMAND_MASTER_DC_SET(commandMaster, frame.statusBits.isCmd);
    COMMAND_MASTER_RW_SET(commandMaster, frame.statusBits.isWrite);

    if (frame.statusBits.isWrite)
    {
        if (frame.statusBits.is16Bits)
        {
            commandMaster->device.TxN16(&(commandMaster->device), frame.buffer.data, frame.buffer.size);
        }
        else
        {
            commandMaster->device.TxN8(&(commandMaster->device), frame.buffer.data, frame.buffer.size);
        }
    }
    else
    {
        if (frame.statusBits.is16Bits)
        {
            commandMaster->device.RxN16(&(commandMaster->device), frame.buffer.data, frame.buffer.size, frame.statusBits.dummyCycles);
        }
        else
        {
            commandMaster->device.RxN8(&(commandMaster->device), frame.buffer.data, frame.buffer.size, frame.statusBits.dummyCycles);
        }
    }
    if (!frame.statusBits.csNotBreak)
    {
        COMMAND_MASTER_CS_DISABLE(commandMaster);
    }
};

DEVICE_STATUS CommandMaster_Init(CommandMaster *commandMaster)
{
    tx_event_flags_create(&(commandMaster->events), "command");

    commandMaster->device.Init(&commandMaster->device);
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS CommandMaster_ConfigCs(CommandMaster *commandMaster, Pin *csPin, COMMAND_SELECT_PIN_MODE config)
{
    commandMaster->csPin = csPin;
    commandMaster->csPinCfg = config;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS CommandMaster_ConfigRw(CommandMaster *commandMaster, Pin *rwPin, COMMAND_READWRITE_PIN_MODE config)
{
    commandMaster->rwPin = rwPin;
    commandMaster->rwPinCfg = config;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS CommandMaster_ConfigDc(CommandMaster *commandMaster, Pin *dcPin, COMMAND_DATACMD_PIN_MODE config)
{
    commandMaster->dcPin = dcPin;
    commandMaster->dcPinCfg = config;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS CommandMaster_SendCommandSync(CommandMaster *commandMaster, CommandFrame *command, uint32_t size)
{
    if (!(commandMaster->device.opMode & COMMAND_DEVICE_OP_MODE_SYNC))
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }
    if (CommandMaster_IsBusy(commandMaster))
    {
        return DEVICE_STATUS_BUSY;
    }

    EVENTS_CLEAR_FLAGS(commandMaster->events);

    EVENTS_SET_FLAGS(commandMaster->events, COMMAND_MASTER_EVENT_CMD_BUSY);

    for (size_t i = 0; i < size; i++)
    {
        CommandMaster_SendFrameSync(commandMaster, command[i]);
    }
    commandMaster->hasError = 0;
    EVENTS_SET_FLAGS(commandMaster->events, COMMAND_MASTER_EVENT_CMD_COMPLETE);
    EVENTS_RESET_FLAGS(commandMaster->events, COMMAND_MASTER_EVENT_CMD_BUSY);
    return DEVICE_STATUS_OK;
};

static void CommandMaster_SendFrameAsync(CommandMaster *commandMaster)
{
    if (commandMaster->_curCommandFrameIndex < commandMaster->_curCommandFrameSize)
    {
        CommandFrame frame = commandMaster->_curCommand[commandMaster->_curCommandFrameIndex];

        COMMAND_MASTER_CS_ENABLE(commandMaster);
        COMMAND_MASTER_DC_SET(commandMaster, frame.statusBits.isCmd);
        COMMAND_MASTER_RW_SET(commandMaster, frame.statusBits.isWrite);

        if (frame.statusBits.isWrite)
        {
            if (frame.statusBits.is16Bits)
            {
                LOG("CMD-SF-A-W:st %lu", commandMaster->_curCommandFrameIndex)
                commandMaster->device.TxN16Async(&(commandMaster->device), frame.buffer.data, frame.buffer.size);
                LOG("CMD-SF-A-W:ed %lu", commandMaster->_curCommandFrameIndex)
            }
            else
            {
                LOG("CMD-SF-A-W:st %lu", commandMaster->_curCommandFrameIndex)
                commandMaster->device.TxN8Async(&(commandMaster->device), frame.buffer.data, frame.buffer.size);
                LOG("CMD-SF-A-W:ed %lu", commandMaster->_curCommandFrameIndex)
            }
        }
        else
        {
            if (frame.statusBits.is16Bits)
            {
                LOG("CMD-SF-A-R:st %lu", commandMaster->_curCommandFrameIndex)
                commandMaster->device.RxN16Async(&(commandMaster->device), frame.buffer.data, frame.buffer.size, frame.statusBits.dummyCycles);
                LOG("CMD-SF-A-R:ed %lu", commandMaster->_curCommandFrameIndex)
            }
            else
            {
                LOG("CMD-SF-A-R:st %lu", commandMaster->_curCommandFrameIndex)
                commandMaster->device.RxN8Async(&(commandMaster->device), frame.buffer.data, frame.buffer.size, frame.statusBits.dummyCycles);
                LOG("CMD-SF-A-R:ed %lu", commandMaster->_curCommandFrameIndex)
            }
        }
    }
    else
    {
        LOG("CMD-SF-E:")
        COMMAND_MASTER_CS_DISABLE(commandMaster);
        commandMaster->hasError = 0;
        EVENTS_SET_FLAGS(commandMaster->events, COMMAND_MASTER_EVENT_CMD_COMPLETE);
        EVENTS_RESET_FLAGS(commandMaster->events, COMMAND_MASTER_EVENT_CMD_BUSY);
    }
};

DEVICE_STATUS CommandMaster_SendCommandAsync(CommandMaster *commandMaster, CommandFrame *command, uint32_t size)
{
    if (!(commandMaster->device.opMode & COMMAND_DEVICE_OP_MODE_ASYNC))
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }

    if (CommandMaster_IsBusy(commandMaster))
    {
        return DEVICE_STATUS_BUSY;
    }

    commandMaster->_curCommand = command;
    commandMaster->_curCommandFrameIndex = 0;
    commandMaster->_curCommandFrameSize = size;

    EVENTS_CLEAR_FLAGS(commandMaster->events);

    EVENTS_SET_FLAGS(commandMaster->events, COMMAND_MASTER_EVENT_CMD_BUSY);
    LOG("CMD-SND-CMD")
    CommandMaster_SendFrameAsync(commandMaster);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS CommandMaster_WaitForComplete(CommandMaster *commandMaster, ULONG timeout)
{
    ULONG actualFlags;
    tx_event_flags_get(&commandMaster->events, COMMAND_MASTER_EVENT_CMD_COMPLETE, TX_AND_CLEAR, &actualFlags, TX_WAIT_FOREVER);
    return commandMaster->hasError ? DEVICE_STATUS_GENERAL_ERROR : DEVICE_STATUS_OK;
};

static inline void CommandMaster_DoTxRxCplt(CommandMaster *commandMaster)
{
    // if (!commandMaster->_curCommand[commandMaster->_curCommandFrameIndex].statusBits.csNotBreak)
    // {
    //     COMMAND_MASTER_CS_DISABLE(commandMaster);
    // }
    commandMaster->_curCommandFrameIndex++;
    CommandMaster_SendFrameAsync(commandMaster);
}

void CommandMaster_DoTxComplete_(CommandMaster *commandMaster)
{
    CommandMaster_DoTxRxCplt(commandMaster);
};

void CommandMaster_DoRxComplete_(CommandMaster *commandMaster)
{

    CommandMaster_DoTxRxCplt(commandMaster);
};

void CommandMaster_DoError_(CommandMaster *commandMaster)
{
    commandMaster->hasError = 1;
    if (CommandMaster_IsBusy(commandMaster))
    {
        EVENTS_RESET_FLAGS(commandMaster->events, COMMAND_MASTER_EVENT_CMD_BUSY);
        EVENTS_SET_FLAGS(commandMaster->events, COMMAND_MASTER_EVENT_CMD_COMPLETE);
    }

    if (commandMaster->onError)
    {
        commandMaster->onError(commandMaster);
    }
};