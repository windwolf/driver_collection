#ifndef ___COMMAND_CLIENT_H__
#define ___COMMAND_CLIENT_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "device.h"
#include "spi_with_pins_device.h"
#include "ringbuffer.h"
#include "tx_api.h"
#include "buffer.h"

#define COMMAND_CLIENT_EVENT_CMD_BUSY 0x01
#define COMMAND_CLIENT_EVENT_CMD_COMPLETE 0x02

    typedef enum COMMAND_SELECT_PIN_MODE
    {
        COMMAND_SELECT_PIN_MODE_SELECT = 0x00,
        COMMAND_SELECT_PIN_MODE_UNSELECT = 0x01,
    } COMMAND_SELECT_PIN_MODE;

    typedef enum COMMAND_DATACMD_PIN_MODE
    {
        COMMAND_DATACMD_PIN_MODE_CMD = 0x00,
        COMMAND_DATACMD_PIN_MODE_DATA = 0x01,

    } COMMAND_DATACMD_PIN_MODE;

    typedef enum COMMAND_READWRITE_PIN_MODE
    {
        COMMAND_READWRITE_PIN_MODE_WRITE = 0x00,
        COMMAND_READWRITE_PIN_MODE_READ = 0x01,

    } COMMAND_READWRITE_PIN_MODE;

    typedef struct CommandFrame
    {
        Buffer buffer;
        union
        {
            uint8_t flags;
            struct
            {
                uint8_t isRead : 1;
                uint8_t isData : 1;
                //uint8_t csNotBreak : 1;
                uint8_t dataBits : 2;
                uint8_t dummyCycles : 4;
            } statusBits;
        };

    } CommandFrame;

    struct CommandClient;
    typedef void (*CommandClientEventHandlerFuncType)(struct CommandClient *commandClient, DEVICE_STATUS error);

    typedef struct CommandClient
    {
        SpiWithPinsDevice *device;
        TX_EVENT_FLAGS_GROUP events;
        uint32_t hasError;
        CommandClientEventHandlerFuncType onError;

        CommandFrame *_curCommandFrame;
        uint32_t _curCommandFrameSize;
        uint32_t _curCommandFrameIndex;

    } CommandClient;

    DEVICE_STATUS command_client_create(CommandClient *commandClient, SpiWithPinsDevice *device);

    void _command_client_register(CommandClient *commandClient, CommandClientEventHandlerFuncType onError);

    DEVICE_STATUS command_client_send(CommandClient *commandClient, CommandFrame *commandFrame, uint32_t size);
    DEVICE_STATUS command_client_cplt_wait(CommandClient *commandClient, ULONG timeout);

#ifdef __cplusplus
}
#endif

#endif // ___COMMAND_CLIENT_H__