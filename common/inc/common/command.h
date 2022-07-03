#ifndef ___COMMAND_H__
#define ___COMMAND_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "device.h"
#include "spi_with_pins_device.h"
#include "ringbuffer.h"
#include "os.h"
#include "buffer.h"

#define COMMAND_READY 0x01UL

    struct Command;
    struct CommandFrame;
    typedef void (*CommandErrorHandleFuncType)(struct Command *command, OP_RESULT error);
    typedef OP_RESULT (*CommandDeviceSendFuncType)(struct Command *command, struct CommandFrame *commandFrame);

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
                // uint32_t commandBits : 2; // always 8bits

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
        DRIVER_EVENTS events;
        uint32_t hasError;
        CommandErrorHandleFuncType onError;

        CommandFrame *_curFrame;
        CommandDeviceSendFuncType _device_send;
    } Command;

    OP_RESULT command_create(Command *command, CommandDeviceSendFuncType deviceSendFunc);
    OP_RESULT command_send(Command *command, CommandFrame *commandFrame);

    OP_RESULT command_cplt_wait(Command *command, uint32_t timeout);

    OP_RESULT command_start(Command *command);
    OP_RESULT command_end(Command *command);

    bool command_is_busy(Command *command);

    typedef struct CommandSpi
    {
        Command base;
        SpiWithPinsDevice *device;
        uint8_t _phase; // 0 = init, 1 = cmd, 2 = addr, 3 = alt, 4 = dummy, 5 = data, 6 = end;

    } CommandSpi;

    OP_RESULT command_spi_create(CommandSpi *command, SpiWithPinsDevice *device);
    void _command_spi_register(CommandSpi *command, CommandErrorHandleFuncType onError);

#ifdef __cplusplus
}
#endif

#endif // ___COMMAND_H__