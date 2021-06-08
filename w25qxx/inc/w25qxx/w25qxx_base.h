#ifndef __w25qxx_base_h__
#define __w25qxx_base_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include "basic/command.h"
#include "basic/buffer.h"
#include "stdint.h"
#include "tx_api.h"

#define W25QXX_EVENT_BUSY 0x01

#define W25QXX_EVENTS_SET_BUSY(instance) EVENTS_SET_FLAGS(instance->events, W25QXX_EVENT_BUSY)
#define W25QXX_EVENTS_RESET_BUSY(instance) EVENTS_RESET_FLAGS(instance->events, W25QXX_EVENT_BUSY);

#define W25QXX_PAGE_SIZE 256

    typedef enum W25QXX_FLASH_ID
    {
        W25QXX_FLASH_ID_SST25VF016B = 0xBF2541,
        W25QXX_FLASH_ID_MX25L1606E = 0xC22015,
        W25QXX_FLASH_ID_W25Q64BV = 0xEF4017, /* BV, JV, FV */
        W25QXX_FLASH_ID_W25Q128 = 0xEF4018
    } W25QXX_FLASH_ID;

    typedef enum W25QXX_STATUS1
    {
        W25QXX_STATUS1_BUSY = 0x01,
        W25QXX_STATUS1_WEL = 0x02,
    } W25QXX_STATUS1;

    typedef struct W25QXX_BASE
    {
        TX_EVENT_FLAGS_GROUP events;
        W25QXX_FLASH_ID flashId;
        char *flashName;
        uint32_t flashSize;
        uint32_t sectorSize;
        Buffer buffer;

    } W25QXX_BASE;

    DEVICE_STATUS w25qxx_create(W25QXX_BASE *instance, Buffer buffer);

#ifdef __cplusplus
}
#endif

#endif //__w25qxx_base_h__