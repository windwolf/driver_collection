#include "basic/uart.h"
#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_dma.h"

#define UART_DEVICE_EVENT_RX_READY 0x01
#define UART_DEVICE_EVENT_TX_COMPLETE 0x02

static UartDevice *UART_INSTANCES[(uint32_t)(UART_COUNT)];

static inline UART_INDEX UartDevice_instance_index_get(UART_HandleTypeDef *huart)
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

static inline void UartDevice_register(Device *device)
{
    UART_INDEX index = UartDevice_instance_index_get(((UartDevice *)device)->handle);
    UART_INSTANCES[(uint32_t)index] = (UartDevice *)device;
}

static inline UartDevice *UartDevice_find(UART_HandleTypeDef *huart)
{
    UART_INDEX index = UartDevice_instance_index_get(huart);
    return UART_INSTANCES[(uint32_t)index];
}

static inline void UartDevice_DoTxComplete(UartDevice *device)
{
    if (device->onTxComplete)
    {
        device->onTxComplete(device);
    }
    tx_event_flags_set(&(device->events), UART_DEVICE_EVENT_TX_COMPLETE, TX_OR);
}

static inline void UartDevice_DoRxReady(UartDevice *device, uint16_t pos)
{
    RingBuffer8_SyncWrite(device->rxBuffer, pos);

    if (device->onRxReady)
    {
        device->onRxReady(device);
    }
    tx_event_flags_set(&(device->events), UART_DEVICE_EVENT_RX_READY, TX_OR);
}

static inline void UartDevice_DoError(UartDevice *device)
{
    if (device->onError)
    {
        device->onError(device);
    }
}

static inline void Uart_TxCpltCallback__(UART_HandleTypeDef *huart)
{
    UartDevice *device = UartDevice_find(huart);
    UartDevice_DoTxComplete(device);
}

static inline void Uart_RxEventCpltCallback__(UART_HandleTypeDef *huart, uint16_t pos)
{
    UartDevice *device = UartDevice_find(huart);
    UartDevice_DoRxReady(device, pos);
}

static inline void Uart_ErrCpltCallback__(UART_HandleTypeDef *huart)
{
    UartDevice *device = UartDevice_find(huart);
    UartDevice_DoError(device);
}

DEVICE_STATUS UartDevice_Init(UartDevice *device, UART_HandleTypeDef *handle, RingBuffer8 *rxBuffer)
{
    assert_param(handle != NULL);
    assert_param(rxBuffer != NULL);

    device->handle = handle;
    device->rxBuffer = rxBuffer;
    tx_event_flags_create(&(device->events), "uart_dev");
    UartDevice_register((Device *)device);
    HAL_UART_RegisterCallback(handle, HAL_UART_TX_COMPLETE_CB_ID, Uart_TxCpltCallback__);
    //HAL_UART_RegisterCallback(_handle, HAL_UART_RX_COMPLETE_CB_ID, Uart_RxCpltCallback__);
    HAL_UART_RegisterCallback(handle, HAL_UART_ERROR_CB_ID, Uart_ErrCpltCallback__);
    HAL_UART_RegisterRxEventCallback(handle, Uart_RxEventCpltCallback__);
    return DEVICE_STATUS_OK;
}

DEVICE_STATUS UartDevice_StartServer(UartDevice *device)
{
    UART_HandleTypeDef *huart = device->handle;
    if (huart->hdmarx->Init.Mode != DMA_CIRCULAR)
    {
        return DEVICE_STATUS_NOT_SUPPORT;
    }
    if ((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX)
    {
        return DEVICE_STATUS_BUSY;
    }

    if (HAL_UARTEx_ReceiveToIdle_DMA(huart, device->rxBuffer->data, RingBuffer8_GetMemorySize(device->rxBuffer)) != HAL_OK)
    {
        UartDevice_DoError(device);
        return DEVICE_STATUS_HARDWARE_ERROR;
    }
    return DEVICE_STATUS_OK;
}

DEVICE_STATUS UartDevice_StopServer(UartDevice *device)
{
    UART_HandleTypeDef *huart = device->handle;
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

DEVICE_STATUS UartDevice_Tx(UartDevice *device, uint8_t *data, uint32_t size)
{
    UART_HandleTypeDef *huart = device->handle;
    if ((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX)
    {
        return DEVICE_STATUS_BUSY;
    }
    if (HAL_UART_Transmit_DMA(huart, data, size) != HAL_OK)
    {
        UartDevice_DoError(device);
        return DEVICE_STATUS_HARDWARE_ERROR;
    }

    return DEVICE_STATUS_OK;
}

UINT UartDevice_WaitForTxComplete(UartDevice *device, ULONG timeout)
{
    ULONG actualFlags;
    return tx_event_flags_get(&device->events, UART_DEVICE_EVENT_TX_COMPLETE, TX_AND_CLEAR, &actualFlags, timeout);
}
/**
     * @brief 阻塞等待rx准备数据.
     * 
     * @param device 
     * @param timeout 
     * @return DEVICE_STATUS 
     */
UINT UartDevice_WaitForRxReady(UartDevice *device, ULONG timeout)
{
    ULONG actualFlags;
    return tx_event_flags_get(&device->events, UART_DEVICE_EVENT_RX_READY, TX_AND_CLEAR, &actualFlags, timeout);
}