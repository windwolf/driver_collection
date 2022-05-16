#include "os.h"

bool driver_mutex_create(DRIVER_MUTEX *mutex, const char *name)
{
    return tx_mutex_create(mutex, name, 0);
}

bool driver_mutex_get(DRIVER_MUTEX *mutex)
{
    return tx_mutex_get(mutex, TX_WAIT_FOREVER) == TX_SUCCESS;
}

bool driver_mutex_put(DRIVER_MUTEX *mutex)
{
    return tx_mutex_put(mutex) == TX_SUCCESS;
}