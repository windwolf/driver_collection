#ifndef __lcd_h__
#define __lcd_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

    typedef enum LCD_COLOR_MODE_ENUM
    {
        MONO,
        RGB565,
        RGB666,
        RGB888,
    } LCD_COLOR_MODE;

    typedef struct LCD_CONFIG_STRUCT
    {
        uint32_t width;
        uint32_t height;
        LCD_COLOR_MODE color_mode;

    } LCD_CONFIG;

    void LCD_Init();

    void LCD_Config();

#ifdef __cplusplus
}
#endif

#endif // __lcd_h__