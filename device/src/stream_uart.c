#include "device/stream_uart.h"

typedef enum UART_INDEX
{
    UART4_INDEX = 0,
    UART5_INDEX,
    UART7_INDEX,
    UART8_INDEX,
    UART_COUNT,
} UART_INDEX;

static Stream *UART_STREAM_INSTANCES[(uint32_t)(UART_COUNT)];

static inline UART_INDEX Stream_instance_index_get(UART_HandleTypeDef *huart)
{
    uint32_t ins = (uint32_t)(huart->Instance);

    switch (ins)
    {
    case UART4_BASE:
        return UART4_INDEX;
    case UART5_BASE:
        return UART5_INDEX;
    case UART7_BASE:
        return UART7_INDEX;
    case UART8_BASE:
        return UART8_INDEX;
    default:
        return UART_COUNT;
    }
}

static inline void Stream_register(StreamDevice *stream)
{
    UART_INDEX index = Stream_instance_index_get((UART_HandleTypeDef *)(stream->instance));
    UART_STREAM_INSTANCES[(uint32_t)index] = (Stream *)stream;
}

static inline Stream *Stream_find(UART_HandleTypeDef *huart)
{
    UART_INDEX index = Stream_instance_index_get(huart);
    return UART_STREAM_INSTANCES[(uint32_t)index];
}

static inline void Uart_TxCpltCallback__(UART_HandleTypeDef *huart)
{
    Stream *stream = Stream_find(huart);
    Stream_DoTxComplete_(stream);
}

static inline void Uart_RxEventCpltCallback__(UART_HandleTypeDef *huart, uint16_t pos)
{
    Stream *stream = Stream_find(huart);
    Stream_DoRxReady_(stream, pos);
}

static inline void Uart_ErrCpltCallback__(UART_HandleTypeDef *huart)
{
    Stream *stream = Stream_find(huart);
    Stream_DoError_(stream);
}

static void Init(StreamDevice *device)
{
    Stream_register(device);
    HAL_UART_RegisterCallback(device->instance, HAL_UART_TX_COMPLETE_CB_ID, Uart_TxCpltCallback__);
    //HAL_UART_RegisterCallback(_handle, HAL_UART_RX_COMPLETE_CB_ID, Uart_RxCpltCallback__);
    HAL_UART_RegisterCallback(device->instance, HAL_UART_ERROR_CB_ID, Uart_ErrCpltCallback__);
    HAL_UART_RegisterRxEventCallback(device->instance, Uart_RxEventCpltCallback__);
}
static DEVICE_STATUS StartRx(StreamDevice *device)
{
    UART_HandleTypeDef *huart = device->instance;
    Stream *stream = (Stream *)device;
    if (huart->hdmarx->Init.Mode != DMA_CIRCULAR)
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }
    if ((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX)
    {
        return DEVICE_STATUS_BUSY;
    }

    if (HAL_UARTEx_ReceiveToIdle_DMA(huart, stream->rxBuffer->data, RingBuffer8_GetMemorySize(stream->rxBuffer)) != HAL_OK)
    {
        return DEVICE_STATUS_HARDWARE_ERROR;
    }
    return DEVICE_STATUS_OK;
}
static DEVICE_STATUS StopRx(StreamDevice *device)
{
    UART_HandleTypeDef *huart = device->instance;
    //Stream *stream = (Stream *)device;
    if ((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_RX) != HAL_UART_STATE_BUSY_RX)
    {
        return DEVICE_STATUS_NOT_NECESSARY;
    }
    if (HAL_UART_DMAStop(huart) != HAL_OK)
    {
        return DEVICE_STATUS_HARDWARE_ERROR;
    }
    return DEVICE_STATUS_OK;
}
static DEVICE_STATUS Tx(StreamDevice *device, uint8_t *data, uint32_t size)
{
    UART_HandleTypeDef *huart = device->instance;
    //Stream *stream = (Stream *)device;
    if ((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX)
    {
        return DEVICE_STATUS_BUSY;
    }
    if (HAL_UART_Transmit_DMA(huart, data, size) != HAL_OK)
    {
        return DEVICE_STATUS_HARDWARE_ERROR;
    }

    return DEVICE_STATUS_OK;
}

void Uart_StreamDevice_Create(StreamDevice *device, UART_HandleTypeDef *huart)
{
    device->instance = huart;
    device->Init = &Init;
    device->StartRx = &StartRx;
    device->StopRx = &StopRx;
    device->Tx = &Tx;
}
