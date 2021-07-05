#include "../inc/common/command.h"

OP_RESULT command_create(Command *command, CommandDeviceSendFuncType deviceSendFunc)
{
    tx_event_flags_create(&(command->events), "5stepcmd");
    command->_device_send = deviceSendFunc;
    command->hasError = 0;
    command->onError = NULL;

    return OP_RESULT_OK;
};

OP_RESULT command_cplt_wait(Command *command, ULONG timeout)
{
    ULONG actualFlags;
    tx_event_flags_get(&command->events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE, TX_AND_CLEAR, &actualFlags, timeout);
    return command->hasError ? OP_RESULT_GENERAL_ERROR : OP_RESULT_OK;
};

OP_RESULT command_send(Command *command, CommandFrame *commandFrame)
{
    return command->_device_send(command, commandFrame);
};