#ifndef ___GX_ST77XX_DRIVER_H__
#define ___GX_ST77XX_DRIVER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "../../device/inc/st77xx/st7735.h"
#include "../../../../guix/common/inc/gx_api.h"

    void st7735_graphics_driver_set(ST77XX *st7735);

    UINT st7735_graphics_driver_rgb565_setup(GX_DISPLAY *display);

#ifdef __cplusplus
}
#endif

#endif // ___GX_ST77XX_DRIVER_H__