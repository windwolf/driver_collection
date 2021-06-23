#ifndef __w25qxx_base_h__
#define __w25qxx_base_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include "../../../common/inc/common/device.h"
#include "../../../common/inc/common/buffer.h"
#include "../../../common/inc/common/command.h"
#include "../../../common/inc/common/block.h"
#include "stdint.h"
#include "tx_api.h"

/* Write Operations */
#define W25QXX_SPI_WRITE_ENABLE_CMD 0x06
#define W25QXX_SPI_VOLATILE_SR_WRITE_ENABLE_CMD 0x50
#define W25QXX_SPI_WRITE_DISABLE_CMD 0x04

/* Identification Operations */
#define W25QXX_SPI_RELEASE_POWER_DOWN_CMD 0xAB
#define W25QXX_SPI_READ_ID_CMD 0x90
#define W25QXX_SPI_READ_JEDEC_ID_CMD 0x9F
#define W25QXX_SPI_READ_UNIQUE_ID_CMD 0x4B

/* Read Operations */
#define W25QXX_SPI_READ_CMD 0x03
#define W25QXX_SPI_FAST_READ_CMD 0x0B

/* Program Operations */
#define W25QXX_SPI_PAGE_PROG_CMD 0x02

/* Erase Operations */
#define W25QXX_SPI_SECTOR_ERASE_4K_CMD 0x20
#define W25QXX_SPI_BLOCK_ERASE_32K_CMD 0x52
#define W25QXX_SPI_BLOCK_ERASE_64K_CMD 0xD8
#define W25QXX_SPI_CHIP_ERASE_CMD 0xC7

/* Regiser Operations */
#define W25QXX_SPI_READ_STATUS_REG1_CMD 0x05
#define W25QXX_SPI_READ_STATUS_REG2_CMD 0x35
#define W25QXX_SPI_READ_STATUS_REG3_CMD 0x15

#define W25QXX_SPI_WRITE_STATUS_REG1_CMD 0x01
#define W25QXX_SPI_WRITE_STATUS_REG2_CMD 0x31
#define W25QXX_SPI_WRITE_STATUS_REG3_CMD 0x11

/* Security Register Operations */
#define W25QXX_SPI_READ_SFDP_REG_CMD 0x5A
#define W25QXX_SPI_ERASE_SECURITY_REG_CMD 0x44
#define W25QXX_SPI_PROGRAM_SECURITY_REG_CMD 0x42
#define W25QXX_SPI_READ_SECURITY_REG_CMD 0x48

/* Lock Operations */
#define W25QXX_SPI_GLOBAL_BLOCK_LOCK_CMD 0x7E
#define W25QXX_SPI_GLOBAL_BLOCK_UNLOCK_CMD 0x98
#define W25QXX_SPI_READ_BLOCK_LOCK_CMD 0x3D
#define W25QXX_SPI_INDEVIDUAL_BLOCK_LOCK_CMD 0x36
#define W25QXX_SPI_INDEVIDUAL_BLOCK_UNLOCK_CMD 0x39

#define W25QXX_SPI_PROG_ERASE_SUSPEND_CMD 0x75
#define W25QXX_SPI_PROG_ERASE_RESUME_CMD 0x7A
#define W25QXX_SPI_PONER_DOWN_CMD 0xB9

#define W25QXX_SPI_ENTER_QPI_MODE_CMD 0x38
#define W25QXX_SPI_ENABLE_RESET_CMD 0x66
#define W25QXX_SPI_RESET_DEVICE_CMD 0x99

#define W25QXX_QSPI_INPUT_PAGE_PROG_CMD 0x32     //1-1-4
#define W25QXX_QSPI_FAST_READ_OUTPUT_CMD 0x6B    //1-1-4
#define W25QXX_QSPI_MFTR_DEVICE_ID_IO_CMD 0x94   //1-4-4
#define W25QXX_QSPI_FAST_READ_IO_CMD 0xEB        //1-4-4
#define W25QXX_QSPI_SET_BURST_WITH_WRAP_CMD 0x77 //1-4-4

/* Write Operations */
#define W25QXX_QPI_WRITE_ENABLE_CMD 0x06
#define W25QXX_QPI_VOLATILE_SR_WRITE_ENABLE_CMD 0x50
#define W25QXX_QPI_WRITE_DISABLE_CMD 0x04

/* Identification Operations */
#define W25QXX_QPI_RELEASE_POWER_DOWN_CMD 0xAB
#define W25QXX_QPI_READ_ID_CMD 0x90
#define W25QXX_QPI_READ_JEDEC_ID_CMD 0x9F
#define W25QXX_QPI_SET_READ_PARAMETERS_CMD 0xC0

/* Read Operations */
#define W25QXX_QPI_FAST_READ_CMD 0x0B
#define W25QXX_QPI_BURST_READ_WITH_WRAP_CMD 0x0C
#define W25QXX_QPI_FAST_READ_QUAD_IO_CMD 0xEB

/* Program Operations */
#define W25QXX_QPI_PAGE_PROG_CMD 0x02

/* Erase Operations */
#define W25QXX_QPI_SECTOR_ERASE_4K_CMD 0x20
#define W25QXX_QPI_BLOCK_ERASE_32K_CMD 0x52
#define W25QXX_QPI_BLOCK_ERASE_64K_CMD 0xD8
#define W25QXX_QPI_CHIP_ERASE_CMD 0xC7

/* Regiser Operations */
#define W25QXX_QPI_READ_STATUS_REG1_CMD 0x05
#define W25QXX_QPI_READ_STATUS_REG2_CMD 0x35
#define W25QXX_QPI_READ_STATUS_REG3_CMD 0x15

#define W25QXX_QPI_WRITE_STATUS_REG1_CMD 0x01
#define W25QXX_QPI_WRITE_STATUS_REG2_CMD 0x31
#define W25QXX_QPI_WRITE_STATUS_REG3_CMD 0x11

/* Lock Operations */
#define W25QXX_QPI_GLOBAL_BLOCK_LOCK_CMD 0x7E
#define W25QXX_QPI_GLOBAL_BLOCK_UNLOCK_CMD 0x7E
#define W25QXX_QPI_READ_BLOCK_LOCK_CMD 0x3D
#define W25QXX_QPI_INDEVIDUAL_BLOCK_LOCK_CMD 0x36
#define W25QXX_QPI_INDEVIDUAL_BLOCK_UNLOCK_CMD 0x39

#define W25QXX_QPI_PROG_ERASE_SUSPEND_CMD 0x75
#define W25QXX_QPI_PROG_ERASE_RESUME_CMD 0x7A
#define W25QXX_QPI_PONER_DOWN_CMD 0xB9

#define W25QXX_QPI_ENTER_QPI_MODE_CMD 0x38
#define W25QXX_QPI_ENABLE_RESET_CMD 0x66
#define W25QXX_QPI_RESET_DEVICE_CMD 0x99
#define W25QXX_QPI_EXIT_QPI_MODE_CMD 0xFF

#define W25QXX_EVENT_OP_BUSY 0x01
#define W25QXX_EVENT_OP_CPLT 0x02

    // #define W25QXX_EVENTS_OP_BUSY_SET(instance) EVENTS_SET_FLAGS(instance->events, W25QXX_EVENT_OP_BUSY)
    // #define W25QXX_EVENTS_OP_BUSY_RESET(instance) EVENTS_RESET_FLAGS(instance->events, W25QXX_EVENT_OP_BUSY)
    // #define W25QXX_EVENTS_DEVICE_BUSY_SET(instance) EVENTS_SET_FLAGS(instance->events, W25QXX_EVENT_DEVICE_BUSY)
    // #define W25QXX_EVENTS_DEVICE_BUSY_RESET(instance) EVENTS_RESET_FLAGS(instance->events, W25QXX_EVENT_DEVICE_BUSY)

#define W25QXX_PAGE_SIZE 256
#define W25QXX_BLOCK_SIZE 4096

    typedef enum W25QXX_STATUS1
    {
        W25QXX_STATUS1_BUSY = 0x01,
        W25QXX_STATUS1_WEL = 0x02,
    } W25QXX_STATUS1;

    typedef enum W25QXX_CMD_MODE
    {
        W25QXX_CMD_MODE_SPI,
        //W25QXX_CMD_MODE_QSPI,
        W25QXX_CMD_MODE_QPI,
    } W25QXX_CMD_MODE;

    typedef struct W25QXX
    {
        TX_EVENT_FLAGS_GROUP events;
        uint16_t mdId;
        uint32_t jedecId;
        W25QXX_CMD_MODE cmdMode;
        union
        {
            uint8_t status1;
            struct
            {
                uint8_t BUSY : 1;
                uint8_t WEL : 1;
                uint8_t BP : 3;
                uint8_t TB : 1;
                uint8_t SEC : 1;
                uint8_t SRP : 1;
            } status1Bits;
        };
        union
        {
            uint8_t status2;
            struct
            {
                uint8_t SRL : 1;
                uint8_t QE : 1;
                uint8_t R1 : 1;
                uint8_t LB : 3;
                uint8_t CMP : 1;
                uint8_t SUS : 1;
            } status2Bits;
        };
        union
        {
            uint8_t status3;
            struct
            {
                uint8_t R3 : 2;
                uint8_t WPS : 1;
                uint8_t R2 : 2;
                uint8_t DRV : 2;
                uint8_t HOLD_RST : 1;
            } status3Bits;
        };
        Command *cc;
        CommandFrame command;
        union
        {
            uint32_t options;
            struct
            {
                uint32_t autoPolling : 1;
                uint32_t dummyCycles : 5;
            };
        };
    } W25QXX;

    DEVICE_STATUS w25qxx_create(W25QXX *instance, Command *cc, uint8_t autoPolling);
    DEVICE_STATUS w25qxx_reset(W25QXX *instance);
    DEVICE_STATUS w25qxx_status_get(W25QXX *instance);
    DEVICE_STATUS w25qxx_status_set(W25QXX *instance);
    DEVICE_STATUS w25qxx_id_read(W25QXX *instance);

    DEVICE_STATUS w25qxx_mode_switch(W25QXX *instance, W25QXX_CMD_MODE cmdMode);

    DEVICE_STATUS w25qxx_read(W25QXX *instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
    DEVICE_STATUS w25qxx_write(W25QXX *instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size);

    DEVICE_STATUS w25qxx_block_erase(W25QXX *instance, uint32_t Address);
    DEVICE_STATUS w25qxx_chip_erase(W25QXX *instance);

    DEVICE_STATUS w25qxx_block_create(W25QXX *instance, Block *block, Buffer buffer);
#ifdef __cplusplus
}
#endif

#endif //__w25qxx_base_h__