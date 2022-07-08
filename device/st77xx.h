#ifndef __st77xx_h__
#define __st77xx_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include "common/command.h"
#include "os/os.h"
#include "stdint.h"
#include "common/device.h"

#define DEVICE_STATUS_POSITION_OUT_OF_RANGE -4

    typedef struct ST77XX
    {
        Command *cc;
        CommandFrame command;
        uint8_t cmdData[16];

        uint8_t pvGamma[16];
        uint8_t nvGamma[16];

        uint16_t xOffset;
        uint16_t width;
        uint16_t yOffset;
        uint16_t height;
        uint8_t colorMode;
        uint8_t orientation;

    } ST77XX;

    OP_RESULT st77xx_create(ST77XX *instance, Command *cc);

    void st77xx_command(ST77XX *instance, uint8_t cmdId);
    // void ST77XX_Command_Write8xN(ST77XX *instance, uint8_t cmdId, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6, uint8_t size);
    // void ST77XX_Command_Write16xN(ST77XX *instance, uint8_t cmdId, uint16_t data1, uint16_t data2, uint8_t size);
    void st77xx_command_write_8(ST77XX *instance, uint8_t cmdId, uint8_t *data, uint16_t size);
    void st77xx_command_write_16(ST77XX *instance, uint8_t cmdId, uint16_t *data, uint16_t size);
    void st77xx_command_read_8(ST77XX *instance, uint8_t cmdId, uint8_t *buffer, uint16_t size);
    void st77xx_command_read_16(ST77XX *instance, uint8_t cmdId, uint16_t *buffer, uint16_t size);

#ifdef __cplusplus
}
#endif
#endif // __st77xx_h__