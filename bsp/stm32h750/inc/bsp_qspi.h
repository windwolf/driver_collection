#ifndef ___BSP_QSPI_H__
#define ___BSP_QSPI_H__ 

#ifdef __cplusplus
extern "C" {
#endif
#include "basic/five_step_command_client.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_qspi.h"

    DEVICE_STATUS five_step_command_client_qspi_create(FiveStepCommandCientQspi *commandClient, QSPI_HandleTypeDef *instance);

#ifdef __cplusplus
}
#endif

#endif	// ___BSP_QSPI_H__

