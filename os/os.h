#ifndef ___OS_H__
#define ___OS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "port.h"
#include <stdbool.h>

    bool driver_mutex_create(DRIVER_MUTEX *mutex, const char *name);

    bool driver_mutex_get(DRIVER_MUTEX *mutex);

    bool driver_mutex_put(DRIVER_MUTEX *mutex);

#ifdef __cplusplus
}
#endif

#endif // ___OS_H__