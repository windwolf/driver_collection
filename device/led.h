#ifndef __WWDEVICE_DEVICE_LED_H__
#define __WWDEVICE_DEVICE_LED_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp.h"
#include "graph.h"

#define TIMER_CHANNEL uint8_t

    typedef struct Led
    {
        TIM_HandleTypeDef *tim;
        TIMER_CHANNEL rChannel;
        TIMER_CHANNEL gChannel;
        TIMER_CHANNEL bChannel;
    } Led;

    void led_create(Led *device, TIM_HandleTypeDef *tim,
                    TIMER_CHANNEL redChannel,
                    TIMER_CHANNEL greenChannel,
                    TIMER_CHANNEL blueChannel);

    void led_color_set(Led *device, Color color);

    void led_start(Led *ledDevice);

    void led_stop(Led *ledDevice);

#ifdef __cplusplus
}
#endif

#endif // __WWDEVICE_DEVICE_LED_H__