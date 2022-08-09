#ifndef __WWDEVICE_DEVICE_LED_H__
#define __WWDEVICE_DEVICE_LED_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp.h"
#include "graph.h"

#define TIMER_CHANNEL uint8_t
#define TIMER_CHANNEL_1 0x01
#define TIMER_CHANNEL_2 0x02
#define TIMER_CHANNEL_3 0x03
#define TIMER_CHANNEL_4 0x04
#define TIMER_CHANNEL_5 0x05
#define TIMER_CHANNEL_6 0x06
#define TIMER_CHANNEL_7 0x07

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