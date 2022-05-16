#include "os.h"

bool driver_mutex_create(DRIVER_MUTEX *mutex, const char *name)
{
    *mutex = 0;
    return true;
}

bool driver_mutex_get(DRIVER_MUTEX *mutex)
{
    while (!*mutex)
    {
    }
    *mutex = 1;
    return true;
}

bool driver_mutex_put(DRIVER_MUTEX *mutex)
{
    *mutex = 0;
    return true;
}