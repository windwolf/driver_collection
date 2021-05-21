#ifndef ___STREAM_UART_H__
#define ___STREAM_UART_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "basic/stream.h"
#include "bsp.h"



    void Uart_StreamDevice_Create(StreamDevice *device, UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif // ___STREAM_UART_H__