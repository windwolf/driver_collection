#include "inc/w25qxx/w25qxx_spi.h"
#include "bsp.h"

DEVICE_STATUS w25qxx_spi_create(W25QXX_SPI *instance, Buffer buffer, SimpleCommand *command)
{
    w25qxx_create(&instance->base, buffer);
    instance->command = command;
    instance->command->flagBits.commandBits = DEVICE_DATAWIDTH_8;
    instance->command->flagBits.addressBits = DEVICE_DATAWIDTH_24;
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_reset(W25QXX_SPI *instance)
{
    SimpleCommand *cmd = instance->command;
    cmd->commandId = W25QXX_SPI_RESET_ENABLE_CMD;

    cmd->flagBits.hasAddress = 0;
    // cmd->flagBits.addressBits = DEVICE_DATAWIDTH_8;
    //cmd->address = 0;

    cmd->dataSize = 0;
    // cmd->flagBits.dataBits = DEVICE_DATAWIDTH_8;
    // cmd->flagBits.isWrite = 1;
    SimpleCommand_SendCommandAsync(cmd);
    CommandBase_WaitForComplete(&cmd->base, TX_WAIT_FOREVER);

    cmd->commandId = W25QXX_SPI_RESET_MEMORY_CMD;

    cmd->dataSize = 0;
    // cmd->flagBits.dataBits = DEVICE_DATAWIDTH_8;
    // cmd->flagBits.isWrite = 1;
    SimpleCommand_SendCommandAsync(cmd);
    CommandBase_WaitForComplete(&cmd->base, TX_WAIT_FOREVER);
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_get_status(W25QXX_SPI *instance)
{
    SimpleCommand *cmd = instance->command;
    cmd->commandId = W25QXX_SPI_READ_STATUS_REG1_CMD;

    cmd->flagBits.hasAddress = 0;
    // cmd->flagBits.addressBits = DEVICE_DATAWIDTH_8;

    cmd->dataSize = 1;
    cmd->flagBits.dataBits = DEVICE_DATAWIDTH_8;
    cmd->flagBits.isWrite = 0;
    cmd->data = instance->base.buffer.data;
    SimpleCommand_SendCommandAsync(cmd);
    CommandBase_WaitForComplete(&cmd->base, TX_WAIT_FOREVER);
    if (*((uint8_t *)(cmd->data)) & W25QXX_STATUS1_BUSY)
    {
        return DEVICE_STATUS_BUSY;
    }
    else
    {
        return DEVICE_STATUS_OK;
    }
};

DEVICE_STATUS w25qxx_spi_write_enable(W25QXX_SPI *instance)
{
    SimpleCommand *cmd = instance->command;
    cmd->commandId = W25QXX_SPI_WRITE_ENABLE_CMD;

    cmd->flagBits.hasAddress = 0;
    // cmd->flagBits.addressBits = DEVICE_DATAWIDTH_8;

    cmd->dataSize = 0;
    // cmd->flagBits.dataBits = DEVICE_DATAWIDTH_8;
    // cmd->flagBits.isWrite = 1;
    SimpleCommand_SendCommandAsync(cmd);
    CommandBase_WaitForComplete(&cmd->base, TX_WAIT_FOREVER);

    while (w25qxx_spi_get_status(instance) == DEVICE_STATUS_BUSY)
    {
        _tx_thread_sleep(1);
    }
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_read_id(W25QXX_SPI *instance, uint32_t *id)
{
    uint32_t id_t = 0;
    SimpleCommand *cmd = instance->command;

    cmd->commandId = W25QXX_SPI_READ_ID_CMD;

    cmd->flagBits.hasAddress = 1;

    cmd->flagBits.isWrite = 0;
    cmd->data = id;
    cmd->dataSize = 1;
    cmd->flagBits.dataBits = DEVICE_DATAWIDTH_24;

    SimpleCommand_SendCommandAsync(cmd);
    CommandBase_WaitForComplete(&cmd->base, TX_WAIT_FOREVER);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_read(W25QXX_SPI *instance, uint8_t *pData, uint32_t readAddr, uint32_t size)
{
    SimpleCommand *cmd = instance->command;

    cmd->commandId = W25QXX_SPI_READ_CMD;

    cmd->flagBits.hasAddress = 1;
    cmd->address = readAddr;

    cmd->flagBits.isWrite = 0;
    cmd->data = pData;
    cmd->dataSize = size;
    cmd->flagBits.dataBits = DEVICE_DATAWIDTH_8;

    SimpleCommand_SendCommandAsync(cmd);
    CommandBase_WaitForComplete(&cmd->base, TX_WAIT_FOREVER);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_write(W25QXX_SPI *instance, uint8_t *pData, uint32_t writeAddr, uint32_t size)
{

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

    w25qxx_spi_write_enable(instance);

    SimpleCommand *cmd = instance->command;
    cmd->commandId = W25QXX_SPI_PAGE_PROG_CMD;

    cmd->flagBits.hasAddress = 1;

    do
    {
        cmd->address = current_addr;
        cmd->flagBits.isWrite = 1;
        cmd->data = pData;
        cmd->dataSize = current_size;
        cmd->flagBits.dataBits = DEVICE_DATAWIDTH_8;
        SimpleCommand_SendCommandAsync(cmd);
        CommandBase_WaitForComplete(&cmd->base, TX_WAIT_FOREVER);

        while (w25qxx_spi_get_status(instance) == DEVICE_STATUS_BUSY)
        {
            tx_thread_sleep(1);
        }
        /* Update the address and size variables for next page programming */
        current_addr += current_size;
        pData += current_size;
        current_size = ((current_addr + W25QXX_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25QXX_PAGE_SIZE;
    } while (current_addr < end_addr);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_erase_block(W25QXX_SPI *instance, uint32_t address)
{

    SimpleCommand *cmd = instance->command;
    cmd->commandId = W25QXX_SPI_SECTOR_ERASE_CMD;

    cmd->flagBits.hasAddress = 1;
    cmd->address = address;

    cmd->dataSize = 0;
    w25qxx_spi_write_enable(instance);

    SimpleCommand_SendCommandAsync(cmd);
    CommandBase_WaitForComplete(&cmd->base, TX_WAIT_FOREVER);

    while (w25qxx_spi_get_status(instance) == DEVICE_STATUS_BUSY)
    {
        tx_thread_sleep(1);
    }

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_erase_chip(W25QXX_SPI *instance)
{
    SimpleCommand *cmd = instance->command;
    cmd->commandId = W25QXX_SPI_CHIP_ERASE_CMD;

    cmd->flagBits.hasAddress = 0;

    cmd->dataSize = 0;
    w25qxx_spi_write_enable(instance);

    SimpleCommand_SendCommandAsync(cmd);
    CommandBase_WaitForComplete(&cmd->base, TX_WAIT_FOREVER);

    while (w25qxx_spi_get_status(instance) == DEVICE_STATUS_BUSY)
    {
        tx_thread_sleep(1);
    }

    return DEVICE_STATUS_OK;
};