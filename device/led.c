#include "led.h"

static void led_enableChannel(Led *device, uint8_t chn)
{
    switch (chn)
    {
    case 1U:
        LL_TIM_CC_EnableChannel(device->tim->Instance, LL_TIM_CHANNEL_CH1);
        break;
    case 2U:
        LL_TIM_CC_EnableChannel(device->tim->Instance, LL_TIM_CHANNEL_CH2);
        break;
    case 3U:
        LL_TIM_CC_EnableChannel(device->tim->Instance, LL_TIM_CHANNEL_CH3);
        break;
    case 4U:
        LL_TIM_CC_EnableChannel(device->tim->Instance, LL_TIM_CHANNEL_CH4);
        break;
    }
}
static void led_disableChannel(Led *device, uint8_t chn)
{
    switch (chn)
    {
    case 1U:
        LL_TIM_CC_DisableChannel(device->tim->Instance, LL_TIM_CHANNEL_CH1);
        break;
    case 2U:
        LL_TIM_CC_DisableChannel(device->tim->Instance, LL_TIM_CHANNEL_CH2);
        break;
    case 3U:
        LL_TIM_CC_DisableChannel(device->tim->Instance, LL_TIM_CHANNEL_CH3);
        break;
    case 4U:
        LL_TIM_CC_DisableChannel(device->tim->Instance, LL_TIM_CHANNEL_CH4);
        break;
    }
}

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

void led_start(Led *device)
{
    // LL_TIM_EnableCounter(ledDevice->tim->Instance);
    led_enableChannel(device, device->rChannel);
    led_enableChannel(device, device->gChannel);
    led_enableChannel(device, device->bChannel);
}

void led_stop(Led *device)
{
    // LL_TIM_DisableCounter(ledDevice->tim->Instance);
    led_disableChannel(device, device->rChannel);
    led_disableChannel(device, device->gChannel);
    led_disableChannel(device, device->bChannel);
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