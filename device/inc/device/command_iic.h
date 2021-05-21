#ifndef ___COMMAND_IIC_H__
#define ___COMMAND_IIC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "basic/command.h"
#include "bsp.h"

    void Spi_CommandDevice_Create(CommandMasterDevice *device, SPI_HandleTypeDef *instance);

#ifdef __cplusplus
}
#endif

#endif // ___COMMAND_IIC_H__