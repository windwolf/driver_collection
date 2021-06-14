#include "../inc/bsp_qspi.h"

static FiveStepCommandClientQspi *_cc;

static DEVICE_STATUS _fscc_qspi_cmd_begin(FiveStepCommandClientQspi *commandClient);
static DEVICE_STATUS _fscc_qspi_cmd_data_txrx(FiveStepCommandClientQspi *commandClient);
static DEVICE_STATUS _fscc_qspi_cmd_data_end(FiveStepCommandClientQspi *commandClient);

static inline BOOL _fscc_qspi_is_busy(FiveStepCommandClientQspi *commandClient)
{
    ULONG actualFlags;
    return tx_event_flags_get(&commandClient->base.events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS;
};

static void _fscc_qspi_cmd_tranlate(CommandStruct *cmd, QSPI_CommandTypeDef *cmdhandler)
{
    cmdhandler->Instruction = cmd->commandId;
    cmdhandler->InstructionMode = cmd->commandMode << QUADSPI_CCR_IMODE_Pos;

    cmdhandler->Address = cmd->address;
    cmdhandler->AddressSize = cmd->addressBits << QUADSPI_CCR_ADSIZE_Pos;
    cmdhandler->AddressMode = cmd->addressMode << QUADSPI_CCR_ADMODE_Pos;

    cmdhandler->AlternateBytes = cmd->altData;
    cmdhandler->AlternateBytesSize = cmd->altDataBits << QSPI_ALTERNATE_BYTES_8_BITS; //QUADSPI_CCR_ABSIZE_Pos
    cmdhandler->AlternateByteMode = cmd->altDataMode << QUADSPI_CCR_ABMODE_Pos;

    cmdhandler->DummyCycles = cmd->dummyCycles;

    cmdhandler->DataMode = cmd->dataMode << QUADSPI_CCR_DMODE_Pos;
    cmdhandler->NbData = cmd->dataSize;

    cmdhandler->DdrMode = cmd->isDdr ? QSPI_DDR_MODE_ENABLE : QSPI_DDR_MODE_DISABLE;
    cmdhandler->DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmdhandler->SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
};

static DEVICE_STATUS _fscc_qspi_cmd_begin(FiveStepCommandClientQspi *commandClient)
{
    commandClient->_phase = 1;

    QSPI_CommandTypeDef cmdHandler;

    _fscc_qspi_cmd_tranlate(commandClient->base._curCmd, &cmdHandler);

    if (commandClient->base._curCmd->dataMode == FIVE_STEP_COMMAND_CLIENT_STEP_MODE_SKIP)
    {
        return HAL_QSPI_Command_IT((QSPI_HandleTypeDef *)commandClient->instance, &cmdHandler);
    }
    else
    {
        DEVICE_STATUS rst = HAL_QSPI_Command_IT((QSPI_HandleTypeDef *)commandClient->instance, &cmdHandler);
        if (rst != HAL_OK)
        {
            return rst;
        }
        return _fscc_qspi_cmd_data_txrx(commandClient);
    }
};

static DEVICE_STATUS _fscc_qspi_cmd_data_txrx(FiveStepCommandClientQspi *commandClient)
{
    commandClient->_phase = 2;
    if (commandClient->base._curCmd->dataMode != FIVE_STEP_COMMAND_CLIENT_STEP_MODE_SKIP)
    {
        if (commandClient->base._curCmd->isWrite)
        {
            if (commandClient->base._curCmd->dataSize > commandClient->dmaThreshold)
            {
                return HAL_QSPI_Transmit_DMA(commandClient->instance, commandClient->base._curCmd->data);
            }
            else
            {
                return HAL_QSPI_Transmit_IT(commandClient->instance, commandClient->base._curCmd->data);
            }
        }
        else
        {
            if (commandClient->base._curCmd->dataSize > commandClient->dmaThreshold)
            {
                return HAL_QSPI_Receive_DMA(commandClient->instance, commandClient->base._curCmd->data);
            }
            else
            {
                return HAL_QSPI_Receive_IT(commandClient->instance, commandClient->base._curCmd->data);
            }
        }
    }
    else
    {
        return _fscc_qspi_cmd_data_end(commandClient);
    }
};

static DEVICE_STATUS _fscc_qspi_cmd_data_end(FiveStepCommandClientQspi *commandClient)
{
    commandClient->_phase = 3;
    EVENTS_RESET_FLAGS(commandClient->base.events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);
    EVENTS_SET_FLAGS(commandClient->base.events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE);
    return DEVICE_STATUS_OK;
};

static DEVICE_STATUS _fscc_qspi_send(FiveStepCommandClient *cc, CommandStruct *command)
{
    FiveStepCommandClientQspi *commandClient = (FiveStepCommandClientQspi *)cc;
    if (_fscc_qspi_is_busy(commandClient))
    {
        return DEVICE_STATUS_BUSY;
    }

    EVENTS_CLEAR_FLAGS(commandClient->base.events);
    EVENTS_SET_FLAGS(commandClient->base.events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);
    commandClient->_phase = 1;
    commandClient->base._curCmd = command;
    return _fscc_qspi_cmd_begin(commandClient);
};

static void _qspi_cmd_cplt(QSPI_HandleTypeDef *instance)
{
    DEVICE_STATUS rst;
    switch (_cc->_phase)
    {
    case 1:
        rst = _fscc_qspi_cmd_data_txrx(_cc);
        break;
    case 2:
        rst = _fscc_qspi_cmd_data_end(_cc);
        break;
    default:
        rst = DEVICE_STATUS_NOT_SUPPORT;
        break;
    }
    if (rst)
    {
        _cc->base.hasError = 1;
        EVENTS_SET_FLAGS(_cc->base.events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE);
        EVENTS_RESET_FLAGS(_cc->base.events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);
    }
};

static void _qspi_error(QSPI_HandleTypeDef *instance)
{
    if (_fscc_qspi_is_busy(_cc))
    {
        EVENTS_RESET_FLAGS(_cc->base.events, FIVE_STEP_COMMAND_EVENT_CMD_BUSY);
        EVENTS_SET_FLAGS(_cc->base.events, FIVE_STEP_COMMAND_EVENT_CMD_COMPLETE);
    }
    if (_cc->base.onError)
    {
        _cc->base.onError((FiveStepCommandClient *)_cc, instance->ErrorCode);
    }
};

static void _fscc_qspi_result_match(QSPI_HandleTypeDef *instance)
{
    EVENTS_SET_FLAGS(_cc->base.events, FSCC_EVENT_AUTO_POLLING_CPLT);
    if (_cc->onStatusPollingResult)
    {
        _cc->onStatusPollingResult(_cc);
    }
};

DEVICE_STATUS fscc_qspi_auto_polling_wait(FiveStepCommandClientQspi *commandClient)
{
    ULONG actualFlags;
    return tx_event_flags_get(&commandClient->base.events, FSCC_EVENT_AUTO_POLLING_CPLT, TX_OR, &actualFlags, TX_WAIT_FOREVER);
};

DEVICE_STATUS fscc_qspi_autopolling(FiveStepCommandClientQspi *commandClient, CommandStruct *pollingCommand, FSCCAutoPollingTypeDef *cfg)
{
    QSPI_CommandTypeDef cmdHandler;
    _fscc_qspi_cmd_tranlate(pollingCommand, &cmdHandler);
    EVENTS_RESET_FLAGS(commandClient->base.events, FSCC_EVENT_AUTO_POLLING_CPLT);
    return HAL_QSPI_AutoPolling((QSPI_HandleTypeDef *)commandClient->instance, &cmdHandler, cfg, HAL_MAX_DELAY);
};

DEVICE_STATUS five_step_command_client_qspi_create(FiveStepCommandClientQspi *commandClient,
                                                   QSPI_HandleTypeDef *instance,
                                                   uint32_t dmaThreshold)
{
    five_step_command_client_create((FiveStepCommandClient *)commandClient, &_fscc_qspi_send);
    commandClient->instance = instance;
    commandClient->dmaThreshold = dmaThreshold;
    HAL_QSPI_RegisterCallback(instance, HAL_QSPI_TX_CPLT_CB_ID, &_qspi_cmd_cplt);
    HAL_QSPI_RegisterCallback(instance, HAL_QSPI_RX_CPLT_CB_ID, &_qspi_cmd_cplt);
    HAL_QSPI_RegisterCallback(instance, HAL_QSPI_CMD_CPLT_CB_ID, &_qspi_cmd_cplt);
    HAL_QSPI_RegisterCallback(instance, HAL_QSPI_STATUS_MATCH_CB_ID, &_fscc_qspi_result_match);
    HAL_QSPI_RegisterCallback(instance, HAL_QSPI_ERROR_CB_ID, &_qspi_error);
    _cc = commandClient;
    return DEVICE_STATUS_OK;
};

void _five_step_command_client_qspi_register(FiveStepCommandClientQspi *commandClient, void *parent,
                                             FiveStepCommandClientErrorHandleFuncType onError,
                                             FiveStepCommandClientQspiEventHandleFuncType onStatusPollingResult)
{
    commandClient->parent = parent;
    commandClient->base.onError = onError;
    commandClient->onStatusPollingResult = onStatusPollingResult;
};
