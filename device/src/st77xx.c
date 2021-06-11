#include "../inc/st77xx/st77xx.h"

DEVICE_STATUS st77xx_create(ST77XX *instance, FiveStepCommandClientSpi *command, Buffer buffer)
{
    instance->command = command;
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
    FiveStepCommandClientSpi *cmd = (FiveStepCommandClientSpi *)instance->command;

    //LOG("ST77XX-CMD0: S: %x", cmdId)
    cmd->base.commandId = cmdId;
    cmd->base.dataSize = 0;
    five_step_command_client_spi_send(cmd);
    //LOG("ST77XX-CMD0: W: %x", cmdId)
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    //LOG("ST77XX-CMD0: WC: %x", cmdId)
}

void st77xx_command_write_8(ST77XX *instance, uint8_t cmdId, uint8_t *data, uint16_t size)
{
    FiveStepCommandClientSpi *cmd = (FiveStepCommandClientSpi *)instance->command;
    cmd->base.commandId = cmdId;
    cmd->base.data = data;
    cmd->base.dataSize = size;
    cmd->base.flagBits.isWrite = 1;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_8;
    //LOG("ST77XX-DATA8: S: %x", cmdId)
    five_step_command_client_spi_send(cmd);
    //LOG("ST77XX-DATA8: W: %x", cmdId)
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    //LOG("ST77XX-DATA8: WC: %x", cmdId)
}

void st77xx_command_write_16(ST77XX *instance, uint8_t cmdId, uint16_t *data, uint16_t size)
{
    FiveStepCommandClientSpi *cmd = (FiveStepCommandClientSpi *)instance->command;
    cmd->base.commandId = cmdId;
    cmd->base.data = data;
    cmd->base.dataSize = size;
    cmd->base.flagBits.isWrite = 1;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_16;
    //LOG("ST77XX-DATA16: S: %x", cmdId)
    five_step_command_client_spi_send(cmd);
    //LOG("ST77XX-DATA16: W: %x", cmdId)
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    //LOG("ST77XX-DATA16: WC: %x", cmdId)
}

void st77xx_command_read_8(ST77XX *instance, uint8_t cmdId, uint16_t size)
{
    FiveStepCommandClientSpi *cmd = (FiveStepCommandClientSpi *)instance->command;
    cmd->base.commandId = cmdId;
    cmd->base.data = instance->buffer.data;
    cmd->base.dataSize = size;
    cmd->base.flagBits.isWrite = 0;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_8;
    //LOG("ST77XX-DATA8: S: %x", cmdId)
    five_step_command_client_spi_send(cmd);
    //LOG("ST77XX-DATA8: W: %x", cmdId)
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    //LOG("ST77XX-DATA8: WC: %x", cmdId)
}

void st77xx_command_read_16(ST77XX *instance, uint8_t cmdId, uint16_t size)
{
    FiveStepCommandClientSpi *cmd = (FiveStepCommandClientSpi *)instance->command;
    cmd->base.commandId = cmdId;
    cmd->base.data = instance->buffer.data;
    cmd->base.dataSize = size;
    cmd->base.flagBits.isWrite = 0;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_16;
    //LOG("ST77XX-DATA16: S: %x", cmdId)
    five_step_command_client_spi_send(cmd);
    //LOG("ST77XX-DATA16: W: %x", cmdId)
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    //LOG("ST77XX-DATA16: WC: %x", cmdId)
}