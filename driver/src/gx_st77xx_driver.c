// #include "../inc/guix/gx_st77xx_driver.h"
// #include "../../../guix/common/inc/gx_display.h"

// static void _st77xx_display_driver_buffer_toggle(struct GX_CANVAS_STRUCT *canvas,
//                                                  GX_RECTANGLE *dirty_area);
// static ST77XX *_instance;

// void st7735_graphics_driver_rgb565_instance_set(ST77XX *instance)
// {
//     _instance = instance;
// }

// UINT st7735_graphics_driver_rgb565_setup(GX_DISPLAY *display)
// {
//     _gx_display_driver_565rgb_setup(display, display->gx_display_driver_data,
//     _st77xx_display_driver_buffer_toggle);
// };

// static void _st77xx_display_driver_buffer_toggle(struct GX_CANVAS_STRUCT *canvas,
//                                                  GX_RECTANGLE *dirty_area)
// {
//     //ST77XX *instance = (ST77XX *)canvas->gx_canvas_display->gx_display_driver_data;

//     //canvas->gx_canvas_memory
//     st7735_rect_draw(_instance,
//                      0, 0,
//                      canvas->gx_canvas_x_resolution - 1, canvas->gx_canvas_y_resolution - 1,
//                      (uint16_t *)canvas->gx_canvas_memory);
// }
