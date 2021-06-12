#include "../inc/w25qxx/w25qxx_spi.h"
#include "bsp.h"
#include <stdlib.h>

static DEVICE_STATUS _w25qxx_spi_busy_wait(W25QXX_SPI *instance)
{
    DEVICE_STATUS rst;
    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_READ_STATUS_REG1_CMD;
    cmd->base.flagBits.hasAddress = 0;
    // cmd->flagBits.addressBits = DEVICE_DATAWIDTH_8;
    cmd->base.dataSize = 1;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_8;
    cmd->base.flagBits.isWrite = 0;
    cmd->base.data = instance->base.buffer.data;
    do
    {
        rst = five_step_command_client_spi_send(cmd);
        if (rst != DEVICE_STATUS_OK)
        {
            return rst;
        }
        rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
        if (rst != DEVICE_STATUS_OK)
        {
            return rst;
        }
        if (*((uint8_t *)(cmd->base.data)) & W25QXX_STATUS1_BUSY)
        {
        }
        else
        {
            return DEVICE_STATUS_OK;
        }
        _tx_thread_sleep(1);
    } while (1);
};

static inline DEVICE_STATUS _w25qxx_spi_read_cmd(FiveStepCommandClientSpi *cmd, uint8_t *pData, uint32_t readAddr, uint32_t size)
{
    DEVICE_STATUS rst;
    cmd->base.commandId = W25QXX_SPI_READ_CMD;

    cmd->base.flagBits.hasAddress = 1;
    cmd->base.address = readAddr;

    cmd->base.flagBits.isWrite = 0;
    cmd->base.data = pData;
    cmd->base.dataSize = size;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_8;

    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    return rst;
}

static inline DEVICE_STATUS _w25qxx_spi_write_enable_cmd(W25QXX_SPI *instance)
{
    DEVICE_STATUS rst;
    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_WRITE_ENABLE_CMD;
    cmd->base.flagBits.hasAddress = 0;
    // cmd->flagBits.addressBits = DEVICE_DATAWIDTH_8;
    cmd->base.dataSize = 0;
    // cmd->flagBits.dataBits = DEVICE_DATAWIDTH_8;
    // cmd->flagBits.isWrite = 1;
    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    return _w25qxx_spi_busy_wait(instance);
};

static inline DEVICE_STATUS _w25qxx_spi_write(W25QXX_SPI *instance, uint8_t *pData, uint32_t writeAddr, uint32_t size)
{
    DEVICE_STATUS rst;

    uint32_t end_addr, current_size, current_addr;
    /* Calculation of the size between the write address and the end of the page */
    current_addr = 0;

    while (current_addr <= writeAddr)
    {
        current_addr += W25QXX_PAGE_SIZE;
    }
    current_size = current_addr - writeAddr;

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > size)
    {
        current_size = size;
    }

    /* Initialize the adress variables */
    current_addr = writeAddr;
    end_addr = writeAddr + size;

    FiveStepCommandClientSpi *cmd = instance->command;

    do
    {
        rst = _w25qxx_spi_write_enable_cmd(instance);
        if (rst != DEVICE_STATUS_OK)
        {
            return rst;
        }
        //w25qxx_spi_read(instance, )
        cmd->base.commandId = W25QXX_SPI_PAGE_PROG_CMD;
        cmd->base.flagBits.hasAddress = 1;
        cmd->base.address = current_addr;
        cmd->base.flagBits.isWrite = 1;
        cmd->base.data = pData;
        cmd->base.dataSize = current_size;
        cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_8;

        rst = five_step_command_client_spi_send(cmd);
        if (rst != DEVICE_STATUS_OK)
        {
            return rst;
        }
        rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
        if (rst != DEVICE_STATUS_OK)
        {
            return rst;
        }
        rst = _w25qxx_spi_busy_wait(instance);
        if (rst != DEVICE_STATUS_OK)
        {
            return rst;
        }
        /* Update the address and size variables for next page programming */
        current_addr += current_size;
        pData += current_size;
        current_size = ((current_addr + W25QXX_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25QXX_PAGE_SIZE;
    } while (current_addr < end_addr);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_create(W25QXX_SPI *instance, Buffer buffer, FiveStepCommandClientSpi *command)
{
    w25qxx_create((W25QXX_BASE *)instance, buffer);

    command->base.flagBits.commandBits = DEVICE_DATAWIDTH_8;
    command->base.flagBits.addressBits = DEVICE_DATAWIDTH_24;
    command->base.flagBits.dummyCycles = 0;
    command->base.flagBits.hasAltData = 0;
    instance->command = command;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_reset(W25QXX_SPI *instance)
{
    DEVICE_STATUS rst;
    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_ENABLE_RESET_CMD;

    cmd->base.flagBits.hasAddress = 0;
    // cmd->flagBits.addressBits = DEVICE_DATAWIDTH_8;
    // cmd->address = 0;

    cmd->base.dataSize = 0;
    // cmd->flagBits.dataBits = DEVICE_DATAWIDTH_8;
    // cmd->flagBits.isWrite = 1;
    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    cmd->base.commandId = W25QXX_SPI_RESET_DEVICE_CMD;

    cmd->base.dataSize = 0;
    // cmd->flagBits.dataBits = DEVICE_DATAWIDTH_8;
    // cmd->flagBits.isWrite = 1;
    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = w25qxx_spi_status_get(instance);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    instance->base.QE = 0;
    rst = w25qxx_spi_status_set(instance);

    return rst;
};

DEVICE_STATUS w25qxx_spi_status_get(W25QXX_SPI *instance)
{
    DEVICE_STATUS rst;
    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_READ_STATUS_REG1_CMD;

    cmd->base.flagBits.hasAddress = 0;

    cmd->base.dataSize = 1;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_8;
    cmd->base.flagBits.isWrite = 0;
    cmd->base.data = instance->base.buffer.data;
    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    instance->base.status1 = *(uint8_t *)(cmd->base.data);
    cmd->base.commandId = W25QXX_SPI_READ_STATUS_REG2_CMD;
    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    instance->base.status2 = *(uint8_t *)(cmd->base.data);
    cmd->base.commandId = W25QXX_SPI_READ_STATUS_REG3_CMD;
    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    instance->base.status3 = *(uint8_t *)(cmd->base.data);
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_status_set(W25QXX_SPI *instance)
{
    DEVICE_STATUS rst;
    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_WRITE_STATUS_REG1_CMD;

    cmd->base.flagBits.hasAddress = 0;

    cmd->base.dataSize = 1;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_8;
    cmd->base.flagBits.isWrite = 1;
    *(uint8_t *)(cmd->base.data) = instance->base.status1;
    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    cmd->base.commandId = W25QXX_SPI_WRITE_STATUS_REG2_CMD;
    *(uint8_t *)(cmd->base.data) = instance->base.status2;
    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    cmd->base.commandId = W25QXX_SPI_WRITE_STATUS_REG3_CMD;
    *(uint8_t *)(cmd->base.data) = instance->base.status3;
    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    return _w25qxx_spi_busy_wait(instance);
};

DEVICE_STATUS w25qxx_spi_id_read(W25QXX_SPI *instance, uint32_t *id)
{
    DEVICE_STATUS rst;
    FiveStepCommandClientSpi *cmd = instance->command;

    cmd->base.commandId = W25QXX_SPI_READ_ID_CMD;

    cmd->base.flagBits.hasAddress = 1;

    cmd->base.flagBits.isWrite = 0;
    cmd->base.data = id;
    cmd->base.dataSize = 1;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_24;

    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    return five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
};

DEVICE_STATUS w25qxx_spi_read(W25QXX_SPI *instance, uint8_t *pData, uint32_t readAddr, uint32_t size)
{
    FiveStepCommandClientSpi *cmd = instance->command;

    return _w25qxx_spi_read_cmd(cmd, pData, readAddr, size);
};

DEVICE_STATUS w25qxx_spi_write(W25QXX_SPI *pInstance, uint8_t *pData, uint32_t writeAddress, uint32_t size)
{
    DEVICE_STATUS rst;
    FiveStepCommandClientSpi *cmd = pInstance->command;
    uint32_t curWriteAddress = writeAddress;
    uint32_t remainSize = size;
    uint32_t blockBeginAddress;
    do
    {
        blockBeginAddress = curWriteAddress & ~(uint32_t)(W25QXX_BLOCK_SIZE);

        uint32_t posInBlock = curWriteAddress & (uint32_t)(W25QXX_BLOCK_SIZE);
        uint32_t sizeInBlock = (size <= curWriteAddress + W25QXX_BLOCK_SIZE - posInBlock) ? size : (curWriteAddress + W25QXX_BLOCK_SIZE - posInBlock);
        rst = _w25qxx_spi_read_cmd(cmd, pInstance->base.buffer.data, blockBeginAddress, W25QXX_BLOCK_SIZE);
        if (rst != DEVICE_STATUS_OK)
        {
            return rst;
        }
        memcpy(pInstance->base.buffer.data + posInBlock, pData, sizeInBlock);
        rst = w25qxx_spi_block_erase(pInstance, blockBeginAddress);
        if (rst != DEVICE_STATUS_OK)
        {
            return rst;
        }
        rst = _w25qxx_spi_write(pInstance, pInstance->base.buffer.data, blockBeginAddress, W25QXX_BLOCK_SIZE);
        if (rst != DEVICE_STATUS_OK)
        {
            return rst;
        }
        curWriteAddress = blockBeginAddress + W25QXX_BLOCK_SIZE;
        remainSize -= sizeInBlock;

    } while (remainSize > 0);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_block_erase(W25QXX_SPI *instance, uint32_t address)
{
    DEVICE_STATUS rst;
    rst = _w25qxx_spi_write_enable_cmd(instance);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_SECTOR_ERASE_4K_CMD;
    cmd->base.flagBits.hasAddress = 1;
    cmd->base.address = address;
    cmd->base.dataSize = 0;

    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    return _w25qxx_spi_busy_wait(instance);
};

DEVICE_STATUS w25qxx_spi_chip_erase(W25QXX_SPI *instance)
{
    DEVICE_STATUS rst;
    rst = _w25qxx_spi_write_enable_cmd(instance);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_CHIP_ERASE_CMD;
    cmd->base.flagBits.hasAddress = 0;
    cmd->base.dataSize = 0;

    rst = five_step_command_client_spi_send(cmd);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    rst = five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    if (rst != DEVICE_STATUS_OK)
    {
        return rst;
    }
    return _w25qxx_spi_busy_wait(instance);
};