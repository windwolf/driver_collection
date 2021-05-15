#ifndef __BASIC_STREAM_H__
#define __BASIC_STREAM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "basic/"

    typedef struct InputStream
    {
        UartDevice device;
    } InputStream;

    void basic_stream_rx_start();

    void basic_stream_rx_stop();

#ifdef __cplusplus
}
#endif

#endif // __BASIC_STREAM_H__