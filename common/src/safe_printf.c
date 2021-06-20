#include "../inc/common/safe_printf.h"
#include "tx_api.h"

static TX_MUTEX _printfMutex;

void safe_printf_init()
{
    tx_mutex_create(&_printfMutex, "_printfMutex", TX_NO_INHERIT);
}

void _begin_safe_printf()
{
    tx_mutex_get(&_printfMutex, TX_WAIT_FOREVER);
}

void _end_safe_printf()
{
    tx_mutex_put(&_printfMutex);
}