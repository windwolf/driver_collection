#include "../inc/w25qxx/w25qxx.h"
#include "bsp.h"
#include <stdlib.h>
#include "string.h"

typedef enum W25QXX_CMD_LINE_MODE
{
    W25QXX_CMD_LINE_MODE_111,
    W25QXX_CMD_LINE_MODE_110,
    W25QXX_CMD_LINE_MODE_101,
    W25QXX_CMD_LINE_MODE_100,
    W25QXX_CMD_LINE_MODE_444,
    W25QXX_CMD_LINE_MODE_440,
    W25QXX_CMD_LINE_MODE_404,
    W25QXX_CMD_LINE_MODE_400,
} W25QXX_CMD_LINE_MODE;

static OP_RESULT _w25qxx_block_erase_wrap(W25QXX *instance, uint32_t addr, uint32_t size)
{
    OP_RESULT rst;
    uint32_t blkBeginAddr = addr & ~(W25QXX_BLOCK_SIZE - 1);
    uint32_t blkEndAddr = (addr + size - 1) & ~(W25QXX_BLOCK_SIZE - 1);
    uint32_t curAddr = blkBeginAddr;
    do
    {
        rst = w25qxx_block_erase(instance, curAddr);
        if (rst != OP_RESULT_OK)
        {
            return rst;
        }
        curAddr += W25QXX_BLOCK_SIZE;

    } while (curAddr <= blkEndAddr);
    return OP_RESULT_OK;
}

static inline void _w25qxx_cmd_line_cfg(CommandFrame *cmd, W25QXX_CMD_LINE_MODE lineMode)
{
    switch (lineMode)
    {
    case W25QXX_CMD_LINE_MODE_111:
        cmd->commandMode = COMMAND_FRAME_MODE_1LINE;
        cmd->addressMode = COMMAND_FRAME_MODE_1LINE;
        cmd->altDataMode = COMMAND_FRAME_MODE_SKIP;
        cmd->dataMode = COMMAND_FRAME_MODE_1LINE;
        break;
    case W25QXX_CMD_LINE_MODE_110:
        cmd->commandMode = COMMAND_FRAME_MODE_1LINE;
        cmd->addressMode = COMMAND_FRAME_MODE_1LINE;
        cmd->altDataMode = COMMAND_FRAME_MODE_SKIP;
        cmd->dataMode = COMMAND_FRAME_MODE_SKIP;
        break;
    case W25QXX_CMD_LINE_MODE_101:
        cmd->commandMode = COMMAND_FRAME_MODE_1LINE;
        cmd->addressMode = COMMAND_FRAME_MODE_SKIP;
        cmd->altDataMode = COMMAND_FRAME_MODE_SKIP;
        cmd->dataMode = COMMAND_FRAME_MODE_1LINE;
        break;
    case W25QXX_CMD_LINE_MODE_100:
        cmd->commandMode = COMMAND_FRAME_MODE_1LINE;
        cmd->addressMode = COMMAND_FRAME_MODE_SKIP;
        cmd->altDataMode = COMMAND_FRAME_MODE_SKIP;
        cmd->dataMode = COMMAND_FRAME_MODE_SKIP;
        break;
    case W25QXX_CMD_LINE_MODE_444:
        cmd->commandMode = COMMAND_FRAME_MODE_4LINE;
        cmd->addressMode = COMMAND_FRAME_MODE_4LINE;
        cmd->altDataMode = COMMAND_FRAME_MODE_SKIP;
        cmd->dataMode = COMMAND_FRAME_MODE_4LINE;
        break;
    case W25QXX_CMD_LINE_MODE_440:
        cmd->commandMode = COMMAND_FRAME_MODE_4LINE;
        cmd->addressMode = COMMAND_FRAME_MODE_4LINE;
        cmd->altDataMode = COMMAND_FRAME_MODE_SKIP;
        cmd->dataMode = COMMAND_FRAME_MODE_SKIP;
        break;
    case W25QXX_CMD_LINE_MODE_404:
        cmd->commandMode = COMMAND_FRAME_MODE_4LINE;
        cmd->addressMode = COMMAND_FRAME_MODE_SKIP;
        cmd->altDataMode = COMMAND_FRAME_MODE_SKIP;
        cmd->dataMode = COMMAND_FRAME_MODE_4LINE;
        break;
    case W25QXX_CMD_LINE_MODE_400:
        cmd->commandMode = COMMAND_FRAME_MODE_4LINE;
        cmd->addressMode = COMMAND_FRAME_MODE_SKIP;
        cmd->altDataMode = COMMAND_FRAME_MODE_SKIP;
        cmd->dataMode = COMMAND_FRAME_MODE_SKIP;
        break;
    default:
        break;
    }
    cmd->dummyCycles = 0;
}

static void _w25qxx_qspi_status_polling_result(W25QXX *instance)
{
    EVENTS_SET_FLAGS(instance->events, W25QXX_EVENT_OP_CPLT);
};

static inline OP_RESULT _w25qxx_op_busy_check(W25QXX *instance)
{
    UINT rst;
    ULONG actualFlags;
    rst = tx_event_flags_get(&instance->events, W25QXX_EVENT_OP_BUSY, TX_AND, &actualFlags, TX_NO_WAIT);
    if (rst == TX_NO_EVENTS)
    {
        return OP_RESULT_OK;
    }
    else
    {
        return OP_RESULT_BUSY;
    }
};

static OP_RESULT _w25qxx_status1_get(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_QPI)
    {
        // QPI
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_404);
    }
    else
    {
        // SPI
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_101);
    }
    cmd->commandId = W25QXX_SPI_READ_STATUS_REG1_CMD;
    cmd->dataSize = 1;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    cmd->isWrite = 0;
    cmd->data = &instance->status1;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return command_cplt_wait(cc, TX_WAIT_FOREVER);
};

static OP_RESULT _w25qxx_status2_get(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_QPI)
    {
        // QPI
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_404);
    }
    else
    {
        // SPI
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_101);
    }
    cmd->commandId = W25QXX_SPI_READ_STATUS_REG2_CMD;
    cmd->dataSize = 1;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    cmd->isWrite = 0;
    cmd->data = &instance->status2;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return command_cplt_wait(cc, TX_WAIT_FOREVER);
};

static OP_RESULT _w25qxx_status3_get(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_QPI)
    {
        // QPI
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_404);
    }
    else
    {
        // SPI
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_101);
    }
    cmd->commandId = W25QXX_SPI_READ_STATUS_REG3_CMD;
    cmd->dataSize = 1;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    cmd->isWrite = 0;
    cmd->data = &instance->status3;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return command_cplt_wait(cc, TX_WAIT_FOREVER);
};

static OP_RESULT _w25qxx_status1_set(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_QPI)
    {
        // QPI
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_404);
    }
    else
    {
        // SPI
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_101);
    }
    cmd->commandId = W25QXX_SPI_WRITE_STATUS_REG1_CMD;
    cmd->dataSize = 1;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    cmd->isWrite = 1;
    cmd->data = &instance->status1;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return command_cplt_wait(cc, TX_WAIT_FOREVER);
};

static OP_RESULT _w25qxx_status2_set(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_QPI)
    {
        // QPI
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_404);
    }
    else
    {
        // SPI
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_101);
    }
    cmd->commandId = W25QXX_SPI_WRITE_STATUS_REG2_CMD;
    cmd->dataSize = 1;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    cmd->isWrite = 1;
    cmd->data = &instance->status2;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return command_cplt_wait(cc, TX_WAIT_FOREVER);
};

static OP_RESULT _w25qxx_status3_set(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_QPI)
    {
        // QPI
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_404);
    }
    else
    {
        // SPI
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_101);
    }
    cmd->commandId = W25QXX_SPI_WRITE_STATUS_REG3_CMD;
    cmd->dataSize = 1;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    cmd->isWrite = 1;
    cmd->data = &instance->status3;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return command_cplt_wait(cc, TX_WAIT_FOREVER);
};

static OP_RESULT _w25qxx_busy_wait(W25QXX *instance)
{
    OP_RESULT rst;
    do
    {
        rst = _w25qxx_status1_get(instance);

        if (rst != OP_RESULT_OK)
        {
            return rst;
        }
        if ((instance->status1 & W25QXX_STATUS1_BUSY) == 0x00)
        {
            return OP_RESULT_OK;
        }
        else
        {
            _tx_thread_sleep(1);
        }
    } while (1);
};

static inline OP_RESULT _w25qxx_read_cmd(W25QXX *instance, uint8_t *pData, uint32_t readAddr, uint32_t size)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_SPI)
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_111);
        cmd->commandId = W25QXX_SPI_READ_CMD;
        cmd->dummyCycles = 0;
    }
    else
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_444);
        cmd->commandId = W25QXX_QPI_FAST_READ_CMD;
        cmd->dummyCycles = instance->dummyCycles;
    }
    cmd->address = readAddr;
    cmd->addressBits = DEVICE_DATAWIDTH_24;

    cmd->data = pData;
    cmd->dataSize = size;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    cmd->isWrite = 0;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = command_cplt_wait(cc, TX_WAIT_FOREVER);
    return rst;
}

static OP_RESULT _w25qxx_read_parameter_set(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_QPI)
    {
        // QPI
        uint8_t params;
        switch (cmd->dummyCycles)
        {
        case 2:
            params = 0x00;
            break;
        case 4:
            params = 0x04;
            break;
        case 6:
            params = 0x08;
            break;
        case 8:
            params = 0x0C;
            break;
        default:
            break;
        }

        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_404);
        cmd->commandId = W25QXX_QPI_SET_READ_PARAMETERS_CMD;
        cmd->dataSize = 1;
        cmd->dataBits = DEVICE_DATAWIDTH_8;
        cmd->isWrite = 1;
        cmd->data = &params;
        rst = command_send(cc, cmd);
        if (rst != OP_RESULT_OK)
        {
            return rst;
        }
        return command_cplt_wait(cc, TX_WAIT_FOREVER);
    }
    else
    {
        // SPI
        return OP_RESULT_NOT_SUPPORT;
    }
};

static inline OP_RESULT _w25qxx_4k_sector_erase_cmd(W25QXX *instance, uint32_t address)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_SPI)
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_110);
    }
    else
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_440);
    }
    cmd->commandId = W25QXX_SPI_SECTOR_ERASE_4K_CMD;
    cmd->address = address;
    cmd->addressBits = DEVICE_DATAWIDTH_24;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = command_cplt_wait(cc, TX_WAIT_FOREVER);
    return rst;
};

static inline OP_RESULT _w25qxx_32k_block_erase_cmd(W25QXX *instance, uint32_t address)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_SPI)
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_110);
    }
    else
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_440);
    }
    cmd->commandId = W25QXX_SPI_BLOCK_ERASE_32K_CMD;
    cmd->address = address;
    cmd->addressBits = DEVICE_DATAWIDTH_24;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = command_cplt_wait(cc, TX_WAIT_FOREVER);
    return rst;
}

static inline OP_RESULT _w25qxx_64k_block_erase_cmd(W25QXX *instance, uint32_t address)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_SPI)
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_110);
    }
    else
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_440);
    }
    cmd->commandId = W25QXX_SPI_BLOCK_ERASE_64K_CMD;
    cmd->address = address;
    cmd->addressBits = DEVICE_DATAWIDTH_24;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = command_cplt_wait(cc, TX_WAIT_FOREVER);
    return rst;
}

static inline OP_RESULT _w25qxx_chip_erase_cmd(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_SPI)
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_100);
    }
    else
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_400);
    }
    cmd->commandId = W25QXX_SPI_CHIP_ERASE_CMD;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = command_cplt_wait(cc, TX_WAIT_FOREVER);
    return rst;
}

static inline OP_RESULT _w25qxx_write_enable_cmd(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_SPI)
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_100);
    }
    else
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_400);
    }
    cmd->commandId = W25QXX_SPI_WRITE_ENABLE_CMD;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return command_cplt_wait(cc, TX_WAIT_FOREVER);
};

static inline OP_RESULT _w25qxx_write_cmd(W25QXX *instance, uint8_t *pData, uint32_t writeAddr, uint32_t size)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_SPI)
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_111);
    }
    else
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_444);
    }

    cmd->commandId = W25QXX_SPI_PAGE_PROG_CMD;
    cmd->address = writeAddr;
    cmd->addressBits = DEVICE_DATAWIDTH_24;
    cmd->isWrite = 1;
    cmd->data = pData;
    cmd->dataSize = size;
    cmd->dataBits = DEVICE_DATAWIDTH_8;

    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return command_cplt_wait(cc, TX_WAIT_FOREVER);
}

static inline OP_RESULT _w25qxx_qpi_enter_cmd(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_100);
    instance->command.commandId = W25QXX_SPI_ENTER_QPI_MODE_CMD;
    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return command_cplt_wait(cc, TX_WAIT_FOREVER);
};

static inline OP_RESULT _w25qxx_qpi_exit_cmd(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_400);
    cmd->commandId = W25QXX_QPI_EXIT_QPI_MODE_CMD;
    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return command_cplt_wait(cc, TX_WAIT_FOREVER);
};

static inline OP_RESULT _w25qxx_reset_cmd(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_SPI)
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_100);
    }
    else
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_400);
    }

    cmd->commandId = W25QXX_QPI_ENABLE_RESET_CMD;
    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = command_cplt_wait(cc, TX_WAIT_FOREVER);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    cmd->commandId = W25QXX_QPI_RESET_DEVICE_CMD;
    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return command_cplt_wait(cc, TX_WAIT_FOREVER);
};

// static inline DEVICE_STATUS _w25qxx_spi_write(W25QXX *instance, uint8_t *pData, uint32_t writeAddr, uint32_t size)
// {
//     DEVICE_STATUS rst;

//     uint32_t end_addr, current_size, current_addr;
//     current_addr = 0;

//     while (current_addr <= writeAddr)
//     {
//         current_addr += W25QXX_PAGE_SIZE;
//     }
//     current_size = current_addr - writeAddr;

//     if (current_size > size)
//     {
//         current_size = size;
//     }

//     current_addr = writeAddr;
//     end_addr = writeAddr + size;

//     do
//     {
//         rst = _w25qxx_write_enable_cmd(instance);
//         if (rst != DEVICE_STATUS_OK)
//         {
//             return rst;
//         }

//         rst = _w25qxx_write_cmd(instance, pData, current_addr, current_size);
//         if (rst != DEVICE_STATUS_OK)
//         {
//             return rst;
//         }

//         rst = _w25qxx_busy_wait(instance);
//         if (rst != DEVICE_STATUS_OK)
//         {
//             return rst;
//         }

//         current_addr += current_size;
//         pData += current_size;
//         current_size = ((current_addr + W25QXX_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25QXX_PAGE_SIZE;
//     } while (current_addr < end_addr);

//     return DEVICE_STATUS_OK;
// };

OP_RESULT w25qxx_create(W25QXX *instance, Command *cc, uint8_t autoPolling)
{
    tx_event_flags_create(&instance->events, "w25qxx");
    instance->cc = cc;
    instance->command.addressBits = DEVICE_DATAWIDTH_24;
    instance->command.dummyCycles = 2;
    instance->autoPolling = autoPolling;
    return OP_RESULT_OK;
};

OP_RESULT w25qxx_mode_switch(W25QXX *instance, W25QXX_CMD_MODE cmdMode)
{
    OP_RESULT rst;
    if (cmdMode == W25QXX_CMD_MODE_SPI)
    {

        if (instance->cmdMode == W25QXX_CMD_MODE_QPI)
        {
            rst = _w25qxx_qpi_exit_cmd(instance);
            if (rst != OP_RESULT_OK)
            {
                return rst;
            }

            instance->cmdMode = cmdMode;
        }
    }
    else
    {
        if (instance->cmdMode == W25QXX_CMD_MODE_SPI)
        {
            if (instance->dummyCycles != 2 &&
                instance->dummyCycles != 4 &&
                instance->dummyCycles != 6 &&
                instance->dummyCycles != 8)
            {
                instance->dummyCycles = 2;
            }
            rst = _w25qxx_status2_get(instance);
            if (rst != OP_RESULT_OK)
            {
                return rst;
            }
            instance->status2Bits.QE = 1;
            rst = _w25qxx_status2_set(instance);
            if (rst != OP_RESULT_OK)
            {
                return rst;
            }
            rst = _w25qxx_qpi_enter_cmd(instance);
            if (rst != OP_RESULT_OK)
            {
                return rst;
            }
            instance->cmdMode = cmdMode;
            rst = _w25qxx_read_parameter_set(instance);
            if (rst != OP_RESULT_OK)
            {
                return rst;
            }
        }
    }
    return OP_RESULT_OK;
};

OP_RESULT w25qxx_reset(W25QXX *instance)
{
    OP_RESULT rst;

    rst = _w25qxx_reset_cmd(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = _w25qxx_qpi_exit_cmd(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = w25qxx_status_get(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    instance->status2Bits.QE = 0;
    rst = _w25qxx_status2_set(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = _w25qxx_busy_wait(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }

    instance->cmdMode = W25QXX_CMD_MODE_SPI;
    return rst;
};

OP_RESULT w25qxx_status_get(W25QXX *instance)
{
    OP_RESULT rst;
    rst = _w25qxx_status1_get(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = _w25qxx_status2_get(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = _w25qxx_status3_get(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return OP_RESULT_OK;
};

OP_RESULT w25qxx_status_set(W25QXX *instance)
{
    OP_RESULT rst;
    rst = _w25qxx_status1_set(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = _w25qxx_busy_wait(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = _w25qxx_status2_set(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = _w25qxx_busy_wait(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = _w25qxx_status3_set(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return _w25qxx_busy_wait(instance);
};

OP_RESULT w25qxx_id_read(W25QXX *instance)
{
    OP_RESULT rst;
    Command *cc = instance->cc;
    CommandFrame *cmd = &instance->command;
    if (instance->cmdMode == W25QXX_CMD_MODE_SPI)
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_111);
    }
    else
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_444);
    }
    cmd->commandId = W25QXX_SPI_READ_ID_CMD;
    cmd->address = 0x000000;
    cmd->addressBits = DEVICE_DATAWIDTH_24;
    cmd->data = &instance->mdId;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    cmd->dataSize = 2;
    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = command_cplt_wait(cc, TX_WAIT_FOREVER);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }

    if (instance->cmdMode == W25QXX_CMD_MODE_SPI)
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_101);
    }
    else
    {
        _w25qxx_cmd_line_cfg(cmd, W25QXX_CMD_LINE_MODE_404);
    }
    cmd->commandId = W25QXX_SPI_READ_JEDEC_ID_CMD;
    cmd->data = &instance->jedecId;
    cmd->dataBits = DEVICE_DATAWIDTH_8;
    cmd->dataSize = 3;
    rst = command_send(cc, cmd);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return command_cplt_wait(cc, TX_WAIT_FOREVER);
};

OP_RESULT w25qxx_read(W25QXX *instance, uint8_t *pData, uint32_t readAddr, uint32_t size)
{
    return _w25qxx_read_cmd(instance, pData, readAddr, size);
};

// DEVICE_STATUS w25qxx_write(W25QXX *instance, uint8_t *pData, uint32_t writeAddress, uint32_t size)
// {
//     DEVICE_STATUS rst;
//     uint32_t curWriteAddress = writeAddress;
//     uint32_t remainSize = size;
//     uint32_t blockBeginAddress;
//     do
//     {
//         blockBeginAddress = curWriteAddress & ~(uint32_t)(W25QXX_BLOCK_SIZE);

//         uint32_t posInBlock = curWriteAddress & (uint32_t)(W25QXX_BLOCK_SIZE);
//         uint32_t sizeInBlock = (size <= curWriteAddress + W25QXX_BLOCK_SIZE - posInBlock) ? size : (curWriteAddress + W25QXX_BLOCK_SIZE - posInBlock);
//         rst = _w25qxx_read_cmd(instance, instance->buffer.data, blockBeginAddress, W25QXX_BLOCK_SIZE);
//         if (rst != DEVICE_STATUS_OK)
//         {
//             return rst;
//         }
//         memcpy(instance->buffer.data + posInBlock, pData, sizeInBlock);
//         rst = w25qxx_block_erase(instance, blockBeginAddress);
//         if (rst != DEVICE_STATUS_OK)
//         {
//             return rst;
//         }
//         rst = _w25qxx_spi_write(instance, instance->buffer.data, blockBeginAddress, W25QXX_BLOCK_SIZE);
//         if (rst != DEVICE_STATUS_OK)
//         {
//             return rst;
//         }
//         curWriteAddress = blockBeginAddress + W25QXX_BLOCK_SIZE;
//         remainSize -= sizeInBlock;

//     } while (remainSize > 0);

//     return DEVICE_STATUS_OK;
// };

OP_RESULT w25qxx_write(W25QXX *instance, uint8_t *pData, uint32_t writeAddr, uint32_t size)
{
    OP_RESULT rst;

    rst = _w25qxx_write_enable_cmd(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }

    rst = _w25qxx_write_cmd(instance, pData, writeAddr, size);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }

    return _w25qxx_busy_wait(instance);
};

OP_RESULT w25qxx_block_erase(W25QXX *instance, uint32_t address)
{
    OP_RESULT rst;
    rst = _w25qxx_write_enable_cmd(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }

    rst = _w25qxx_4k_sector_erase_cmd(instance, address);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return _w25qxx_busy_wait(instance);
};

OP_RESULT w25qxx_chip_erase(W25QXX *instance)
{
    OP_RESULT rst;
    rst = _w25qxx_write_enable_cmd(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    rst = _w25qxx_chip_erase_cmd(instance);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return _w25qxx_busy_wait(instance);
};

OP_RESULT w25qxx_block_create(W25QXX *instance, Block *block, Buffer buffer)
{
    block_create(block, instance,
                 0, W25QXX_PAGE_SIZE, W25QXX_BLOCK_SIZE,
                 true,
                 BLOCK_MODE_RANDOM, BLOCK_MODE_WRAP, BLOCK_MODE_RANDOM_BLOCK,
                 buffer,
                 &w25qxx_read, &w25qxx_write, &_w25qxx_block_erase_wrap);
    return OP_RESULT_OK;
};