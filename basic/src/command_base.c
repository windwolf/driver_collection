#include "basic/command.h"
#include "basic/shared.h"

DEVICE_STATUS command_base_create(CommandBase *commandBase, PacketIoDevice *device,
                                  Pin *csPin, COMMAND_SELECT_PIN_MODE csCfg,
                                  Pin *rwPin, COMMAND_READWRITE_PIN_MODE rwCfg,
                                  Pin *dcPin, COMMAND_DATACMD_PIN_MODE dcCfg)
{
    commandBase->device = device;
    tx_event_flags_create(&(commandBase->events), "command");
    commandBase->csPin = csPin;
    commandBase->csPinCfg = csCfg;
    commandBase->rwPin = rwPin;
    commandBase->rwPinCfg = rwCfg;
    commandBase->dcPin = dcPin;
    commandBase->dcPinCfg = dcCfg;
    return DEVICE_STATUS_OK;
};

uint8_t CommandBase_IsBusy(CommandBase *commandBase)
{
    ULONG actualFlags;
    return tx_event_flags_get(&commandBase->events, COMMAND_BASE_EVENT_CMD_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS;
};

DEVICE_STATUS CommandBase_WaitForComplete(CommandBase *commandBase, ULONG timeout)
{
    ULONG actualFlags;
    tx_event_flags_get(&commandBase->events, COMMAND_BASE_EVENT_CMD_COMPLETE, TX_AND_CLEAR, &actualFlags, TX_WAIT_FOREVER);
    return commandBase->hasError ? DEVICE_STATUS_GENERAL_ERROR : DEVICE_STATUS_OK;
};
