#include "bsp_qspi.h"

static FiveStepCommandCientQspi *_cc;

static inline BOOL _is_busy(FiveStepCommandClient *commandClient)
{
    ULONG actualFlags;
    return tx_event_flags_get(&commandClient->events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS;
};

static void _qspi_cmd_cplt(QSPI_HandleTypeDef *instance)
{
    _cc->base.hasError = 0;
    EVENTS_SET_FLAGS(_cc->base.events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE);
    EVENTS_RESET_FLAGS(_cc->base.events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);
};
static void _qspi_error(QSPI_HandleTypeDef *instance)
{
    if (_is_busy((FiveStepCommandClient *)_cc))
    {
        EVENTS_RESET_FLAGS(_cc->base.events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);
        EVENTS_SET_FLAGS(_cc->base.events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE);
    }
    if (_cc->base.onError)
    {
        _cc->base.onError((FiveStepCommandClient *)_cc, instance->ErrorCode);
    }
};

DEVICE_STATUS five_step_command_client_qspi_create(FiveStepCommandCientQspi *commandClient,
                                                   QSPI_HandleTypeDef *instance)
{
    five_step_command_client_create((FiveStepCommandClient *)commandClient);
    commandClient->instance = instance;
    HAL_QSPI_RegisterCallback(instance, HAL_QSPI_CMD_CPLT_CB_ID, &_qspi_cmd_cplt);
    HAL_QSPI_RegisterCallback(instance, HAL_QSPI_ERROR_CB_ID, &_qspi_error);
    _cc = commandClient;
    return DEVICE_STATUS_OK;
};
void _five_step_command_client_qspi_register(FiveStepCommandCientQspi *commandClient,
                                             FiveStepCommandClientEventHandlerFuncType onError)
{
    commandClient->base.onError = onError;
};

DEVICE_STATUS five_step_command_client_qspi_send(FiveStepCommandCientQspi *commandClient)
{

    if (_is_busy((FiveStepCommandClient *)commandClient))
    {
        return DEVICE_STATUS_BUSY;
    }

    EVENTS_CLEAR_FLAGS(commandClient->base.events);

    EVENTS_SET_FLAGS(commandClient->base.events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);

    QSPI_CommandTypeDef Cmdhandler;

    Cmdhandler.Instruction = commandClient->base.commandId;
    Cmdhandler.InstructionMode = commandClient->lineMode.commandLines << QUADSPI_CCR_IMODE_Pos;

    Cmdhandler.Address = commandClient->base.address;
    Cmdhandler.AddressSize = commandClient->base.flagBits.addressBits << QUADSPI_CCR_ADSIZE_Pos;
    Cmdhandler.AddressMode = commandClient->lineMode.addressLines << QUADSPI_CCR_ADMODE_Pos;

    Cmdhandler.AlternateBytes = commandClient->base.altData;
    Cmdhandler.AlternateBytesSize = commandClient->base.flagBits.altDataBits << QSPI_ALTERNATE_BYTES_8_BITS; //QUADSPI_CCR_ABSIZE_Pos
    Cmdhandler.AlternateByteMode = commandClient->lineMode.altDataLines << QUADSPI_CCR_ABMODE_Pos;
    Cmdhandler.DummyCycles = commandClient->base.flagBits.dummyCycles;

    Cmdhandler.DataMode = commandClient->lineMode.dataLines << QUADSPI_CCR_DMODE_Pos;
    Cmdhandler.NbData = commandClient->base.dataSize;

    Cmdhandler.DdrMode = QSPI_DDR_MODE_DISABLE;
    Cmdhandler.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    Cmdhandler.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    return HAL_QSPI_Command_IT((QSPI_HandleTypeDef *)commandClient->instance, &Cmdhandler);
};
