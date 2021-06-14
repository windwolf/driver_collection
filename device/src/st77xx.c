#include "../inc/st77xx/st77xx.h"

DEVICE_STATUS st77xx_create(ST77XX *instance, FiveStepCommandClient *cc, Buffer buffer)
{
    instance->cc = cc;
    instance->buffer = buffer;
    tx_event_flags_create(&instance->events, "st77xx");
    return DEVICE_STATUS_OK;
}

int st77xx_is_busy(ST77XX *instance)
{
    ULONG actualFlags;
    return tx_event_flags_get(&instance->events, ST77XX_EVENT_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS;
};

DEVICE_STATUS st77xx_lock(ST77XX *instance)
{
    if (st77xx_is_busy(instance))
    {
        return DEVICE_STATUS_BUSY;
    }
    EVENTS_SET_FLAGS(instance->events, ST77XX_EVENT_BUSY);
    //LOG("ST77XX: LCK")
    return DEVICE_STATUS_OK;
}

void st77xx_unlock(ST77XX *instance)
{
    EVENTS_RESET_FLAGS(instance->events, ST77XX_EVENT_BUSY);
    //LOG("ST77XX: UNLCK")
}

void st77xx_command(ST77XX *instance, uint8_t cmdId)
{
    FiveStepCommandClient *cc = instance->cc;
    CommandStruct *cmd = &instance->command;
    //LOG("ST77XX-CMD0: S: %x", cmdId)
    cmd->commandId = cmdId;
    cmd->dataSize = 0;
    five_step_command_client_send(cc, cmd);
    //LOG("ST77XX-CMD0: W: %x", cmdId)
    five_step_command_client_cplt_wait(cc, TX_WAIT_FOREVER);
    //LOG("ST77XX-CMD0: WC: %x", cmdId)
}

void st77xx_command_write_8(ST77XX *instance, uint8_t cmdId, uint8_t *data, uint16_t size)
{
    FiveStepCommandClient *cc = instance->cc;
    CommandStruct *cmd = &instance->command;
    cmd->commandId = cmdId;
    cmd->data = data;
    cmd->dataSize = size;
    cmd->isWrite = 1;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    //LOG("ST77XX-DATA8: S: %x", cmdId)
    five_step_command_client_send(cc, cmd);
    //LOG("ST77XX-DATA8: W: %x", cmdId)
    five_step_command_client_cplt_wait(cc, TX_WAIT_FOREVER);
    //LOG("ST77XX-DATA8: WC: %x", cmdId)
}

void st77xx_command_write_16(ST77XX *instance, uint8_t cmdId, uint16_t *data, uint16_t size)
{
    FiveStepCommandClient *cc = instance->cc;
    CommandStruct *cmd = &instance->command;
    cmd->commandId = cmdId;
    cmd->data = data;
    cmd->dataSize = size;
    cmd->isWrite = 1;
    cmd->dataBits = DEVICE_DATAWIDTH_16;
    //LOG("ST77XX-DATA16: S: %x", cmdId)
    five_step_command_client_send(cc, cmd);
    //LOG("ST77XX-DATA16: W: %x", cmdId)
    five_step_command_client_cplt_wait(cc, TX_WAIT_FOREVER);
    //LOG("ST77XX-DATA16: WC: %x", cmdId)
}

void st77xx_command_read_8(ST77XX *instance, uint8_t cmdId, uint16_t size)
{
    FiveStepCommandClient *cc = instance->cc;
    CommandStruct *cmd = &instance->command;
    cmd->commandId = cmdId;
    cmd->data = instance->buffer.data;
    cmd->dataSize = size;
    cmd->isWrite = 0;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    //LOG("ST77XX-DATA8: S: %x", cmdId)
    five_step_command_client_send(cc, cmd);
    //LOG("ST77XX-DATA8: W: %x", cmdId)
    five_step_command_client_cplt_wait(cc, TX_WAIT_FOREVER);
    //LOG("ST77XX-DATA8: WC: %x", cmdId)
}

void st77xx_command_read_16(ST77XX *instance, uint8_t cmdId, uint16_t size)
{
    FiveStepCommandClient *cc = instance->cc;
    CommandStruct *cmd = &instance->command;
    cmd->commandId = cmdId;
    cmd->data = instance->buffer.data;
    cmd->dataSize = size;
    cmd->isWrite = 0;
    cmd->dataBits = DEVICE_DATAWIDTH_16;
    //LOG("ST77XX-DATA16: S: %x", cmdId)
    five_step_command_client_send(cc, cmd);
    //LOG("ST77XX-DATA16: W: %x", cmdId)
    five_step_command_client_cplt_wait(cc, TX_WAIT_FOREVER);
    //LOG("ST77XX-DATA16: WC: %x", cmdId)
}