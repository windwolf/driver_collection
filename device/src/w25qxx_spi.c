#include "../inc/w25qxx/w25qxx_spi.h"
#include "bsp.h"

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
    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_ENABLE_RESET_CMD;

    cmd->base.flagBits.hasAddress = 0;
    // cmd->flagBits.addressBits = DEVICE_DATAWIDTH_8;
    //cmd->address = 0;

    cmd->base.dataSize = 0;
    // cmd->flagBits.dataBits = DEVICE_DATAWIDTH_8;
    // cmd->flagBits.isWrite = 1;
    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);

    cmd->base.commandId = W25QXX_SPI_RESET_DEVICE_CMD;

    cmd->base.dataSize = 0;
    // cmd->flagBits.dataBits = DEVICE_DATAWIDTH_8;
    // cmd->flagBits.isWrite = 1;
    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);

    w25qxx_spi_status_get(instance);
    instance->base.QE = 0;
    w25qxx_spi_status_set(instance);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_status_get(W25QXX_SPI *instance)
{
    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_READ_STATUS_REG1_CMD;

    cmd->base.flagBits.hasAddress = 0;

    cmd->base.dataSize = 1;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_8;
    cmd->base.flagBits.isWrite = 0;
    cmd->base.data = instance->base.buffer.data;
    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    instance->base.status1 = *(uint8_t *)(cmd->base.data);
    cmd->base.commandId = W25QXX_SPI_READ_STATUS_REG2_CMD;
    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    instance->base.status2 = *(uint8_t *)(cmd->base.data);
    cmd->base.commandId = W25QXX_SPI_READ_STATUS_REG3_CMD;
    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    instance->base.status3 = *(uint8_t *)(cmd->base.data);
    return DEVICE_STATUS_BUSY;
};

DEVICE_STATUS w25qxx_spi_status_set(W25QXX_SPI *instance)
{
    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_WRITE_STATUS_REG1_CMD;

    cmd->base.flagBits.hasAddress = 0;

    cmd->base.dataSize = 1;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_8;
    cmd->base.flagBits.isWrite = 1;
    *(uint8_t *)(cmd->base.data) = instance->base.status1;
    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);

    cmd->base.commandId = W25QXX_SPI_WRITE_STATUS_REG2_CMD;
    *(uint8_t *)(cmd->base.data) = instance->base.status2;
    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);

    cmd->base.commandId = W25QXX_SPI_WRITE_STATUS_REG3_CMD;
    *(uint8_t *)(cmd->base.data) = instance->base.status3;
    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    return DEVICE_STATUS_BUSY;
};

DEVICE_STATUS w25qxx_spi_is_busy(W25QXX_SPI *instance)
{
    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_READ_STATUS_REG1_CMD;

    cmd->base.flagBits.hasAddress = 0;
    // cmd->flagBits.addressBits = DEVICE_DATAWIDTH_8;

    cmd->base.dataSize = 1;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_8;
    cmd->base.flagBits.isWrite = 0;
    cmd->base.data = instance->base.buffer.data;
    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);
    if (*((uint8_t *)(cmd->base.data)) & W25QXX_STATUS1_BUSY)
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
    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_WRITE_ENABLE_CMD;

    cmd->base.flagBits.hasAddress = 0;
    // cmd->flagBits.addressBits = DEVICE_DATAWIDTH_8;

    cmd->base.dataSize = 0;
    // cmd->flagBits.dataBits = DEVICE_DATAWIDTH_8;
    // cmd->flagBits.isWrite = 1;
    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);

    while (w25qxx_spi_is_busy(instance) == DEVICE_STATUS_BUSY)
    {
        _tx_thread_sleep(1);
    }
    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_id_read(W25QXX_SPI *instance, uint32_t *id)
{
    FiveStepCommandClientSpi *cmd = instance->command;

    cmd->base.commandId = W25QXX_SPI_READ_ID_CMD;

    cmd->base.flagBits.hasAddress = 1;

    cmd->base.flagBits.isWrite = 0;
    cmd->base.data = id;
    cmd->base.dataSize = 1;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_24;

    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_read(W25QXX_SPI *instance, uint8_t *pData, uint32_t readAddr, uint32_t size)
{
    FiveStepCommandClientSpi *cmd = instance->command;

    cmd->base.commandId = W25QXX_SPI_READ_CMD;

    cmd->base.flagBits.hasAddress = 1;
    cmd->base.address = readAddr;

    cmd->base.flagBits.isWrite = 0;
    cmd->base.data = pData;
    cmd->base.dataSize = size;
    cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_8;

    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_write(W25QXX_SPI *instance, uint8_t *pData, uint32_t writeAddr, uint32_t size)
{
    w25qxx_spi_write_enable(instance);

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
    cmd->base.commandId = W25QXX_SPI_PAGE_PROG_CMD;

    cmd->base.flagBits.hasAddress = 1;

    do
    {
        cmd->base.address = current_addr;
        cmd->base.flagBits.isWrite = 1;
        cmd->base.data = pData;
        cmd->base.dataSize = current_size;
        cmd->base.flagBits.dataBits = DEVICE_DATAWIDTH_8;
        five_step_command_client_spi_send(cmd);
        five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);

        while (w25qxx_spi_is_busy(instance) == DEVICE_STATUS_BUSY)
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

DEVICE_STATUS w25qxx_spi_block_erase(W25QXX_SPI *instance, uint32_t address)
{

    w25qxx_spi_write_enable(instance);

    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_SECTOR_ERASE_4K_CMD;
    cmd->base.flagBits.hasAddress = 1;
    cmd->base.address = address;
    cmd->base.dataSize = 0;

    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);

    while (w25qxx_spi_is_busy(instance) == DEVICE_STATUS_BUSY)
    {
        tx_thread_sleep(1);
    }

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS w25qxx_spi_chip_erase(W25QXX_SPI *instance)
{
    w25qxx_spi_write_enable(instance);

    FiveStepCommandClientSpi *cmd = instance->command;
    cmd->base.commandId = W25QXX_SPI_CHIP_ERASE_CMD;
    cmd->base.flagBits.hasAddress = 0;
    cmd->base.dataSize = 0;

    five_step_command_client_spi_send(cmd);
    five_step_command_client_cplt_wait((FiveStepCommandClient *)cmd, TX_WAIT_FOREVER);

    while (w25qxx_spi_is_busy(instance) == DEVICE_STATUS_BUSY)
    {
        tx_thread_sleep(1);
    }

    return DEVICE_STATUS_OK;
};