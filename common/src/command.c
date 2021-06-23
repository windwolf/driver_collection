#include "../inc/common/command.h"

DEVICE_STATUS command_create(Command *command, CommandDeviceSendFuncType deviceSendFunc)
{
    tx_event_flags_create(&(command->events), "5stepcmd");
    command->_device_send = deviceSendFunc;
    command->hasError = 0;
    command->onError = NULL;

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS command_cplt_wait(Command *command, ULONG timeout)
{
    ULONG actualFlags;
    tx_event_flags_get(&command->events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE, TX_AND_CLEAR, &actualFlags, timeout);
    return command->hasError ? DEVICE_STATUS_GENERAL_ERROR : DEVICE_STATUS_OK;
};

DEVICE_STATUS command_send(Command *command, CommandFrame *commandFrame)
{
    return command->_device_send(command, commandFrame);
};