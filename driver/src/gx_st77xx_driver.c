#include "../inc/guix/gx_st77xx_driver.h"

static ST77XX *instance;
static void _st77xx_display_driver_buffer_toggle(struct GX_CANVAS_STRUCT *canvas,
                                                 GX_RECTANGLE *dirty_area);

void st7735_graphics_driver_set(ST77XX *st7735)
{
    instance = st7735;
};

UINT st7735_graphics_driver_rgb565_setup(GX_DISPLAY *display)
{
    display->gx_display_driver_buffer_toggle;
};

static void _st77xx_display_driver_buffer_toggle(struct GX_CANVAS_STRUCT *canvas,
                                                 GX_RECTANGLE *dirty_area)
{
    //canvas->gx_canvas_memory
    st7735_rect_draw(instance,
                     dirty_area->gx_rectangle_left,
                     dirty_area->gx_rectangle_top,
                     dirty_area->gx_rectangle_right,
                     dirty_area->gx_rectangle_top);
}
