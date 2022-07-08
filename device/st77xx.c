#include "st77xx.h"

static void _st77xx_command_setup(CommandFrame *cmd)
{
    cmd->commandMode = COMMAND_FRAME_MODE_1LINE;
    cmd->addressMode = COMMAND_FRAME_MODE_SKIP;
    cmd->altDataMode = COMMAND_FRAME_MODE_SKIP;
    cmd->dataMode = COMMAND_FRAME_MODE_1LINE;
    cmd->dummyCycles = 0;
}

OP_RESULT st77xx_create(ST77XX *instance, Command *cc)
{
    instance->cc = cc;
    return OP_RESULT_OK;
}

void st77xx_command(ST77XX *instance, uint8_t cmdId)
{
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    _st77xx_command_setup(cmd);
    // LOG("ST77XX-CMD0: S: %x", cmdId)
    cmd->commandId = cmdId;
    cmd->dataSize = 0;
    command_send(cc, cmd);
    // LOG("ST77XX-CMD0: W: %x", cmdId)
    command_cplt_wait(cc, DRIVER_TIMEOUT_FOREVER);
    // LOG("ST77XX-CMD0: WC: %x", cmdId)
}

void st77xx_command_write_8(ST77XX *instance, uint8_t cmdId, uint8_t *data, uint16_t size)
{
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    _st77xx_command_setup(cmd);
    cmd->commandId = cmdId;
    cmd->data = data;
    cmd->dataSize = size;
    cmd->isWrite = 1;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    // LOG("ST77XX-DATA8: S: %x", cmdId)
    command_send(cc, cmd);
    // LOG("ST77XX-DATA8: W: %x", cmdId)
    command_cplt_wait(cc, DRIVER_TIMEOUT_FOREVER);
    // LOG("ST77XX-DATA8: WC: %x", cmdId)
}

void st77xx_command_write_16(ST77XX *instance, uint8_t cmdId, uint16_t *data, uint16_t size)
{
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    _st77xx_command_setup(cmd);
    cmd->commandId = cmdId;
    cmd->data = data;
    cmd->dataSize = size;
    cmd->isWrite = 1;
    cmd->dataBits = DEVICE_DATAWIDTH_16;
    // LOG("ST77XX-DATA16: S: %x", cmdId)
    command_send(cc, cmd);
    // LOG("ST77XX-DATA16: W: %x", cmdId)
    command_cplt_wait(cc, DRIVER_TIMEOUT_FOREVER);
    // LOG("ST77XX-DATA16: WC: %x", cmdId)
}

void st77xx_command_read_8(ST77XX *instance, uint8_t cmdId, uint8_t *buffer, uint16_t size)
{
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    _st77xx_command_setup(cmd);
    cmd->commandId = cmdId;
    cmd->data = buffer;
    cmd->dataSize = size;
    cmd->isWrite = 0;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    // LOG("ST77XX-DATA8: S: %x", cmdId)
    command_send(cc, cmd);
    // LOG("ST77XX-DATA8: W: %x", cmdId)
    command_cplt_wait(cc, DRIVER_TIMEOUT_FOREVER);
    // LOG("ST77XX-DATA8: WC: %x", cmdId)
}

void st77xx_command_read_16(ST77XX *instance, uint8_t cmdId, uint16_t *buffer, uint16_t size)
{
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    _st77xx_command_setup(cmd);
    cmd->commandId = cmdId;
    cmd->data = buffer;
    cmd->dataSize = size;
    cmd->isWrite = 0;
    cmd->dataBits = DEVICE_DATAWIDTH_16;
    // LOG("ST77XX-DATA16: S: %x", cmdId)
    command_send(cc, cmd);
    // LOG("ST77XX-DATA16: W: %x", cmdId)
    command_cplt_wait(cc, DRIVER_TIMEOUT_FOREVER);
    // LOG("ST77XX-DATA16: WC: %x", cmdId)
}