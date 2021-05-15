#ifndef __BASIC_UART_H__
#define __BASIC_UART_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_uart.h"
#include "basic/device.h"
#include "basic/ring_buffer8.h"
#include "tx_api.h"

    typedef enum UART_INDEX
    {
        UART4_INDEX = 0,
        UART5_INDEX,
        UART7_INDEX,
        UART8_INDEX,
        UART_COUNT,
    } UART_INDEX;

    typedef struct UartDevice
    {
        Device base;
        UART_HandleTypeDef *handle;
        RingBuffer8 *rxBuffer;
        TX_EVENT_FLAGS_GROUP events;
        void (*onTxComplete)(struct UartDevice *device);
        void (*onRxReady)(struct UartDevice *device);
        void (*onError)(struct UartDevice *device);
    } UartDevice;

    DEVICE_STATUS UartDevice_Init(UartDevice *device, UART_HandleTypeDef *handle, RingBuffer8 *rxBuffer);

    DEVICE_STATUS UartDevice_StartServer(UartDevice *device);

    DEVICE_STATUS UartDevice_StopServer(UartDevice *device);

    DEVICE_STATUS UartDevice_Tx(UartDevice *device, void *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // __BASIC_UART_H__
