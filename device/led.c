#include "led.h"

void led_create(Led *device,
                TIM_HandleTypeDef *tim,
                TIMER_CHANNEL redChannel,
                TIMER_CHANNEL greenChannel,
                TIMER_CHANNEL blueChannel)
{
    device->tim = tim;
    device->rChannel = redChannel;
    device->gChannel = greenChannel;
    device->bChannel = blueChannel;
};

void led_start(Led *ledDevice)
{
    LL_TIM_EnableCounter(ledDevice->tim->Instance);
}

void led_stop(Led *ledDevice)
{
    LL_TIM_DisableCounter(ledDevice->tim->Instance);
}

void led_color_set(Led *device, Color color)
{
    uint32_t ar = LL_TIM_GetAutoReload(device->tim->Instance);
    switch (device->rChannel)
    {
    case 1U:
        LL_TIM_OC_SetCompareCH1(device->tim->Instance, ar * color.ARGB.r / 256);
        break;
    case 2U:
        LL_TIM_OC_SetCompareCH2(device->tim->Instance, ar * color.ARGB.r / 256);
        break;
    case 3U:
        LL_TIM_OC_SetCompareCH3(device->tim->Instance, ar * color.ARGB.r / 256);
        break;
    case 4U:
        LL_TIM_OC_SetCompareCH4(device->tim->Instance, ar * color.ARGB.r / 256);
        break;
    }

    switch (device->gChannel)
    {
    case 1U:
        LL_TIM_OC_SetCompareCH1(device->tim->Instance, ar * color.ARGB.g / 256);
        break;
    case 2U:
        LL_TIM_OC_SetCompareCH2(device->tim->Instance, ar * color.ARGB.g / 256);
        break;
    case 3U:
        LL_TIM_OC_SetCompareCH3(device->tim->Instance, ar * color.ARGB.g / 256);
        break;
    case 4U:
        LL_TIM_OC_SetCompareCH4(device->tim->Instance, ar * color.ARGB.g / 256);
        break;
    }

    switch (device->bChannel)
    {
    case 1U:
        LL_TIM_OC_SetCompareCH1(device->tim->Instance, ar * color.ARGB.b / 256);
        break;
    case 2U:
        LL_TIM_OC_SetCompareCH2(device->tim->Instance, ar * color.ARGB.b / 256);
        break;
    case 3U:
        LL_TIM_OC_SetCompareCH3(device->tim->Instance, ar * color.ARGB.b / 256);
        break;
    case 4U:
        LL_TIM_OC_SetCompareCH4(device->tim->Instance, ar * color.ARGB.b / 256);
        break;
    }
}