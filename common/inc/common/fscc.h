#ifndef ___FIVE_STEP_COMMAND_CLIENT_H__
#define ___FIVE_STEP_COMMAND_CLIENT_H__

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

    struct FiveStepCommandClient;
struct CommandStruct;
    typedef void (*FiveStepCommandClientErrorHandleFuncType)(struct FiveStepCommandClient *commandClient, DEVICE_STATUS error);
    typedef DEVICE_STATUS (*FiveStepCommandClientDeviceSendFuncType)(struct FiveStepCommandClient *commandClient, struct CommandStruct *command);

    typedef enum FIVE_STEP_COMMAND_CLIENT_STEP_MODE
    {
        FIVE_STEP_COMMAND_CLIENT_STEP_MODE_SKIP = 0,
        FIVE_STEP_COMMAND_CLIENT_STEP_MODE_1LINE = 1,
        FIVE_STEP_COMMAND_CLIENT_STEP_MODE_2LINE = 2,
        FIVE_STEP_COMMAND_CLIENT_STEP_MODE_4LINE = 3,
    } FIVE_STEP_COMMAND_CLIENT_STEP_MODE;

    typedef struct CommandStruct
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
    } CommandStruct;

    typedef struct FiveStepCommandClient
    {
        TX_EVENT_FLAGS_GROUP events;
        uint32_t hasError;
        FiveStepCommandClientErrorHandleFuncType onError;

        CommandStruct *_curCmd;
        FiveStepCommandClientDeviceSendFuncType _device_send;
    } FiveStepCommandClient;
    DEVICE_STATUS five_step_command_client_create(FiveStepCommandClient *commandClient, FiveStepCommandClientDeviceSendFuncType deviceSendFunc);
    DEVICE_STATUS five_step_command_client_cplt_wait(FiveStepCommandClient *commandClient, ULONG timeout);
    DEVICE_STATUS five_step_command_client_send(FiveStepCommandClient *commandClient, CommandStruct *command);

    typedef struct FiveStepCommandClientSpi
    {
        FiveStepCommandClient base;
        SpiWithPinsDevice *device;
        uint8_t _phase; // 0 = init, 1 = cmd, 2 = addr, 3 = alt, 4 = dummy, 5 = data, 6 = end;

    } FiveStepCommandClientSpi;

    DEVICE_STATUS five_step_command_client_spi_create(FiveStepCommandClientSpi *commandClient, SpiWithPinsDevice *device);
    void _five_step_command_client_spi_register(FiveStepCommandClientSpi *commandClient, FiveStepCommandClientErrorHandleFuncType onError);

#ifdef __cplusplus
}
#endif

#endif // ___FIVE_STEP_COMMAND_CLIENT_H__