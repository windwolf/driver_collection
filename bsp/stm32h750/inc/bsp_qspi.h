#ifndef ___BSP_QSPI_H__
#define ___BSP_QSPI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "../../../common/inc/common/device.h"
#include "../../../common/inc/common/fscc.h"
#include "stm32h7xx_hal.h"

#define FSCCAutoPollingTypeDef QSPI_AutoPollingTypeDef
#define FSCC_EVENT_AUTO_POLLING_CPLT 0x08000000
struct FiveStepCommandClientQspi;
    typedef void (*FiveStepCommandClientQspiEventHandleFuncType)(struct FiveStepCommandClientQspi *instance);
    typedef struct FiveStepCommandClientQspi
    {
        FiveStepCommandClient base;
        void *instance;
        uint32_t dmaThreshold;

        void* parent;
        FiveStepCommandClientQspiEventHandleFuncType onStatusPollingResult;

        Buffer _rxBuffer;
        struct
        {
            uint8_t isDmaTx : 1;
            uint8_t isDmaRx : 1;
        } _status;
    } FiveStepCommandClientQspi;

    void _five_step_command_client_qspi_register(FiveStepCommandClientQspi *commandClient, void *parent,
                                                 FiveStepCommandClientErrorHandleFuncType onError,
                                                 FiveStepCommandClientQspiEventHandleFuncType onStatusPollingResult);

    DEVICE_STATUS five_step_command_client_qspi_create(FiveStepCommandClientQspi *commandClient,
                                                       QSPI_HandleTypeDef *instance,
                                                       uint32_t dmaThreshold);

    DEVICE_STATUS fscc_qspi_autopolling(FiveStepCommandClientQspi *commandClient, CommandStruct* pollingCommand, FSCCAutoPollingTypeDef *autoPolling);

#ifdef __cplusplus
}
#endif

#endif // ___BSP_QSPI_H__
