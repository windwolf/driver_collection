#ifndef ___FIVE_STEP_COMMAND_H__
#define ___FIVE_STEP_COMMAND_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "basic/device.h"
#include "basic/spi_with_pins_device.h"
#include "basic/ringbuffer.h"
#include "tx_api.h"
#include "basic/buffer.h"

#define FIVE_STEP_COMMAND_EVENT_CMD_BUSY 0x01
#define FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE 0x02

    struct FiveStepCommandClient;

    typedef void (*FiveStepCommandClientEventHandlerFuncType)(struct FiveStepCommandClient *commandClient, DEVICE_STATUS error);

    typedef struct FiveStepCommandClient
    {
        TX_EVENT_FLAGS_GROUP events;
        uint32_t hasError;
        FiveStepCommandClientEventHandlerFuncType onError;

        uint32_t commandId;
        uint32_t address;
        uint32_t altData;
        void *data;
        uint16_t dataSize;
        union
        {
            uint32_t flags;
            struct
            {
                uint32_t hasCommand : 1;
                uint32_t commandBits : 2;
                uint32_t hasAddress : 1;
                uint32_t addressBits : 2;
                uint32_t hasAltData : 1;
                uint32_t altDataBits : 2;

                uint32_t isWrite : 1;
                uint32_t dataBits : 2;
                uint32_t dummyCycles : 5;

            } flagBits;
        };

    } FiveStepCommandClient;

    typedef struct FiveStepCommandClientSpi
    {
        FiveStepCommandClient base;
        SpiWithPinsDevice *device;
        uint8_t _phase; // 0 = init, 1 = cmd, 2 = addr, 3 = alt, 4 = dummy, 5 = data, 6 = end;

    } FiveStepCommandClientSpi;

    DEVICE_STATUS five_step_command_client_create(FiveStepCommandClient *commandClient);
    DEVICE_STATUS five_step_command_client_cplt_wait(FiveStepCommandClient *commandClient, ULONG timeout);

    DEVICE_STATUS five_step_command_client_spi_create(FiveStepCommandClientSpi *commandClient, SpiWithPinsDevice *device);

    void _five_step_command_client_spi_register(FiveStepCommandClientSpi *commandClient,
                                                FiveStepCommandClientEventHandlerFuncType onError);

    DEVICE_STATUS five_step_command_client_spi_send(FiveStepCommandClientSpi *commandClient);

    typedef struct FiveStepCommandCientQspi
    {
        FiveStepCommandClient base;
        void *instance;
        struct
        {
            uint8_t commandLines : 2;
            uint8_t addressLines : 2;
            uint8_t altDataLines : 2;
            uint8_t dataLines : 2;
        } lineMode;
    } FiveStepCommandCientQspi;

    void _five_step_command_client_qspi_register(FiveStepCommandCientQspi *commandClient,
                                                 FiveStepCommandClientEventHandlerFuncType onError);

    DEVICE_STATUS five_step_command_client_qspi_cplt_wait(FiveStepCommandCientQspi *commandClient, ULONG timeout);

    DEVICE_STATUS five_step_command_client_qspi_send(FiveStepCommandCientQspi *commandClient);

#ifdef __cplusplus
}
#endif

#endif // ___FIVE_STEP_COMMAND_H__