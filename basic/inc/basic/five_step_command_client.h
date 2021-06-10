#ifndef ___FIVE_STEP_COMMAND_H__
#define ___FIVE_STEP_COMMAND_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "basic/device.h"
#include "basic/device_ex.h"
#include "basic/ring_buffer8.h"
#include "tx_api.h"
#include "basic/buffer.h"

#define FIVE_STEP_COMMAND_EVENT_CMD_BUSY 0x01
#define FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE 0x02

    struct FiveStepCommandCient;

    typedef void (*FiveStepCommandCientEventHandlerFuncType)(struct FiveStepCommandCient *commandClient, DEVICE_STATUS error);

    typedef struct FiveStepCommandCient
    {
        SpiWithPinsDevice *device;

        TX_EVENT_FLAGS_GROUP events;
        uint32_t hasError;
        FiveStepCommandCientEventHandlerFuncType onError;
        void *data;
        void *altData;
        uint32_t commandId;
        uint32_t address;
        uint16_t dataSize;
        uint16_t altDataSize;
        union
        {
            uint16_t flags;
            struct
            {

                uint16_t commandBits : 2;
                uint16_t addressBits : 2;
                uint16_t altDataBits : 2;
                uint16_t hasAddress : 1;
                uint16_t hasCommand : 1;
                uint16_t isWrite : 1;
                uint16_t dataBits : 2;
                uint16_t dummyCycles : 4;

            } flagBits;
        };
        uint8_t _phase; // 0 = init, 1 = cmd, 2 = addr, 3 = alt, 4 = dummy, 5 = data, 6 = end;

    } FiveStepCommandCient;

    DEVICE_STATUS five_step_command_client_create(FiveStepCommandCient *commandClient, SpiWithPinsDevice *device);

    void _five_step_command_client_register(FiveStepCommandCient *commandClient,
                                            FiveStepCommandCientEventHandlerFuncType onError);

    DEVICE_STATUS five_step_command_client_cplt_wait(FiveStepCommandCient *commandClient, ULONG timeout);

    DEVICE_STATUS five_step_command_client_send(FiveStepCommandCient *commandClient);

#ifdef __cplusplus
}
#endif

#endif // ___FIVE_STEP_COMMAND_H__