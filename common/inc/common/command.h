#ifndef ___COMMAND_H__
#define ___COMMAND_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "device.h"
#include "spi_with_pins_device.h"
#include "ringbuffer.h"
#include "tx_api.h"
#include "buffer.h"

#define FIVE_STEP_COMMAND_EVENT_CMD_BUSY 0x01
#define FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE 0x02

    struct Command;
    struct CommandFrame;
    typedef void (*CommandErrorHandleFuncType)(struct Command *command, DEVICE_STATUS error);
    typedef DEVICE_STATUS (*CommandDeviceSendFuncType)(struct Command *command, struct CommandFrame *commandFrame);

    typedef enum COMMAND_FRAME_MODE
    {
        COMMAND_FRAME_MODE_SKIP = 0,
        COMMAND_FRAME_MODE_1LINE = 1,
        COMMAND_FRAME_MODE_2LINE = 2,
        COMMAND_FRAME_MODE_4LINE = 3,
    } COMMAND_FRAME_MODE;

    typedef struct CommandFrame
    {
        ALIGN32 uint8_t commandId;
        uint32_t address;
        uint32_t altData;
        void *data;
        uint16_t dataSize;
        union
        {
            uint32_t flags;
            struct
            {
                uint32_t commandMode : 2;
                //uint32_t commandBits : 2; // always 8bits

                uint32_t addressMode : 2;
                uint32_t addressBits : 2;

                uint32_t altDataMode : 2;
                uint32_t altDataBits : 2;

                uint32_t dataMode : 2;
                uint32_t dataBits : 2;
                uint32_t isWrite : 1;

                uint32_t dummyCycles : 5;

                uint32_t isDdr : 1;
            };
        };
    } CommandFrame;

    typedef struct Command
    {
        TX_EVENT_FLAGS_GROUP events;
        uint32_t hasError;
        CommandErrorHandleFuncType onError;

        CommandFrame *_curFrame;
        CommandDeviceSendFuncType _device_send;
    } Command;
    DEVICE_STATUS command_create(Command *command, CommandDeviceSendFuncType deviceSendFunc);
    DEVICE_STATUS command_cplt_wait(Command *command, ULONG timeout);
    DEVICE_STATUS command_send(Command *command, CommandFrame *commandFrame);

    typedef struct CommandSpi
    {
        Command base;
        SpiWithPinsDevice *device;
        uint8_t _phase; // 0 = init, 1 = cmd, 2 = addr, 3 = alt, 4 = dummy, 5 = data, 6 = end;

    } CommandSpi;

    DEVICE_STATUS command_spi_create(CommandSpi *command, SpiWithPinsDevice *device);
    void _command_spi_register(CommandSpi *command, CommandErrorHandleFuncType onError);

#ifdef __cplusplus
}
#endif

#endif // ___COMMAND_H__