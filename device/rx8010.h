#ifndef __WWDEVICE_RX8010_H__
#define __WWDEVICE_RX8010_H__

#include "common/device.h"
#include "temporal/temporal.h"

typedef struct RX8010
{
    I2CDevice *i2c;

} RX8010;

OP_RESULT rx8010_init(RX8010 *rx8010, I2CDevice *i2c);
OP_RESULT rx8010_datetime_Get(RX8010 *rx8010, DateTime *datetime);
#endif // __WWDEVICE_RX8010_H__