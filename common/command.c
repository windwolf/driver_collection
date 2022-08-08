#include "common/command.h"

OP_RESULT command_create(Command *command, CommandDeviceSendFuncType deviceSendFunc)
{
    ww_os_events_create(&(command->events), "5stepcmd");
    ww_os_events_set(&(command->events), COMMAND_READY);
    command->_device_send = deviceSendFunc;
    command->hasError = 0;
    command->onError = NULL;

    return OP_RESULT_OK;
};

OP_RESULT command_cplt_wait(Command *command, uint32_t timeout)
{
    if (!ww_os_events_get(&(command->events), COMMAND_READY, DRIVER_EVENTS_OPTION_AND, timeout))
    {
        return OP_RESULT_BUSY;
    }
    // ww_os_events_reset(&(command->events), COMMAND_READY);
    return command->hasError ? OP_RESULT_GENERAL_ERROR : OP_RESULT_OK;
};

OP_RESULT command_send(Command *command, CommandFrame *commandFrame)
{
    return command->_device_send(command, commandFrame);
};

OP_RESULT command_start(Command *command)
{
    ww_os_events_reset(&(command->events), COMMAND_READY);
    return OP_RESULT_OK;
};

OP_RESULT command_end(Command *command)
{
    ww_os_events_set(&(command->events), COMMAND_READY);
    return OP_RESULT_OK;
};

bool command_is_busy(Command *command)
{
    return !ww_os_events_get(&(command->events), COMMAND_READY, DRIVER_EVENTS_OPTION_AND, command->timeout);
};
