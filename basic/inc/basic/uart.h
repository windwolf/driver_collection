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
        /**
         * @brief tx完成后的回调. @note 回调运行在中断上下文中, 注意控制时间. 
         * 
         */
        void (*onTxComplete)(struct UartDevice *device);
        /**
         * @brief rx有值时的回调. @note 回调运行在中断上下文中, 注意控制时间. 
         * 
         */
        void (*onRxReady)(struct UartDevice *device);
        void (*onError)(struct UartDevice *device);
    } UartDevice;

    DEVICE_STATUS UartDevice_Init(UartDevice *device, UART_HandleTypeDef *handle, RingBuffer8 *rxBuffer);

    DEVICE_STATUS UartDevice_StartServer(UartDevice *device);

    DEVICE_STATUS UartDevice_StopServer(UartDevice *device);

    DEVICE_STATUS UartDevice_Tx(UartDevice *device, uint8_t *data, uint32_t size);
    /**
     * @brief 阻塞等待tx完成.
     * 
     * @param device 
     * @return DEVICE_STATUS 
     */
    UINT UartDevice_WaitForTxComplete(UartDevice *device, ULONG timeout);
    /**
     * @brief 阻塞等待rx准备数据.
     * 
     * @param device 
     * @param timeout 
     * @return DEVICE_STATUS 
     */
    UINT UartDevice_WaitForRxReady(UartDevice *device, ULONG timeout);

#ifdef __cplusplus
}
#endif

#endif // __BASIC_UART_H__
