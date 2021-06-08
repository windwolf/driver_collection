#ifndef ___W25QXX_SPI_H__
#define ___W25QXX_SPI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "w25qxx_base.h"

#define W25QXX_SPI_RESET_ENABLE_CMD 0x66
#define W25QXX_SPI_RESET_MEMORY_CMD 0x99

#define W25QXX_SPI_ENTER_QPI_MODE_CMD 0x38
#define W25QXX_SPI_EXIT_QPI_MODE_CMD 0xFF

/* Identification Operations */
#define W25QXX_SPI_READ_ID_CMD 0x90
#define W25QXX_SPI_DUAL_READ_ID_CMD 0x92
#define W25QXX_SPI_QUAD_READ_ID_CMD 0x94
#define W25QXX_SPI_READ_JEDEC_ID_CMD 0x9F

/* Read Operations */
#define W25QXX_SPI_READ_CMD 0x03
#define W25QXX_SPI_FAST_READ_CMD 0x0B
#define W25QXX_SPI_DUAL_OUT_FAST_READ_CMD 0x3B
#define W25QXX_SPI_DUAL_INOUT_FAST_READ_CMD 0xBB
#define W25QXX_SPI_QUAD_OUT_FAST_READ_CMD 0x6B
#define W25QXX_SPI_QUAD_INOUT_FAST_READ_CMD 0xEB

/* Write Operations */
#define W25QXX_SPI_WRITE_ENABLE_CMD 0x06
#define W25QXX_SPI_WRITE_DISABLE_CMD 0x04

/* Regiser Operations */
#define W25QXX_SPI_READ_STATUS_REG1_CMD 0x05
#define W25QXX_SPI_READ_STATUS_REG2_CMD 0x35
#define W25QXX_SPI_READ_STATUS_REG3_CMD 0x15

#define W25QXX_SPI_WRITE_STATUS_REG1_CMD 0x01
#define W25QXX_SPI_WRITE_STATUS_REG2_CMD 0x31
#define W25QXX_SPI_WRITE_STATUS_REG3_CMD 0x11

/* Program Operations */
#define W25QXX_SPI_PAGE_PROG_CMD 0x02
#define W25QXX_SPI_QUAD_INPUT_PAGE_PROG_CMD 0x32

/* Erase Operations */
#define W25QXX_SPI_SECTOR_ERASE_CMD 0x20
#define W25QXX_SPI_CHIP_ERASE_CMD 0xC7

#define W25QXX_SPI_PROG_ERASE_RESUME_CMD 0x7A
#define W25QXX_SPI_PROG_ERASE_SUSPEND_CMD 0x75



    typedef struct W25QXX_SPI
    {
        W25QXX_BASE base;
        SimpleCommand *command;
    } W25QXX_SPI;

    DEVICE_STATUS w25qxx_spi_create(W25QXX_SPI *instance, Buffer buffer, SimpleCommand* command);
    DEVICE_STATUS w25qxx_spi_reset(W25QXX_SPI* instance);
    DEVICE_STATUS w25qxx_spi_get_status(W25QXX_SPI* instance);
    DEVICE_STATUS w25qxx_spi_write_enable(W25QXX_SPI* instance);
    DEVICE_STATUS w25qxx_spi_read_id(W25QXX_SPI* instance, uint32_t *id);
    DEVICE_STATUS w25qxx_spi_read(W25QXX_SPI* instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
    DEVICE_STATUS w25qxx_spi_write(W25QXX_SPI* instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
    DEVICE_STATUS w25qxx_spi_erase_block(W25QXX_SPI* instance, uint32_t Address);
    DEVICE_STATUS w25qxx_spi_erase_chip(W25QXX_SPI* instance);

#ifdef __cplusplus
}
#endif

#endif // ___W25QXX_SPI_H__