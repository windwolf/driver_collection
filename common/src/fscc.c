#include "../inc/common/fscc.h"

DEVICE_STATUS five_step_command_client_create(FiveStepCommandClient *commandClient, FiveStepCommandClientDeviceSendFuncType deviceSendFunc)
{
    tx_event_flags_create(&(commandClient->events), "5stepcmd");
    commandClient->_device_send = deviceSendFunc;
    commandClient->hasError = 0;
    commandClient->onError = NULL;

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS five_step_command_client_cplt_wait(FiveStepCommandClient *commandClient, ULONG timeout)
{
    ULONG actualFlags;
    tx_event_flags_get(&commandClient->events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE, TX_AND_CLEAR, &actualFlags, TX_WAIT_FOREVER);
    return commandClient->hasError ? DEVICE_STATUS_GENERAL_ERROR : DEVICE_STATUS_OK;
};

DEVICE_STATUS five_step_command_client_send(FiveStepCommandClient *commandClient, CommandStruct *command)
{
    return commandClient->_device_send(commandClient, command);
};