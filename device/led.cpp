#include "led.hpp"
#include "stm32g0xx_ll_tim.h"

namespace ww::device
{

static void led_enableChannel(TIM_HandleTypeDef &tim, uint8_t chn)
{
    switch (chn)
    {
    case 1U:
        LL_TIM_CC_EnableChannel(tim.Instance, LL_TIM_CHANNEL_CH1);
        break;
    case 2U:
        LL_TIM_CC_EnableChannel(tim.Instance, LL_TIM_CHANNEL_CH2);
        break;
    case 3U:
        LL_TIM_CC_EnableChannel(tim.Instance, LL_TIM_CHANNEL_CH3);
        break;
    case 4U:
        LL_TIM_CC_EnableChannel(tim.Instance, LL_TIM_CHANNEL_CH4);
        break;
    }
}
static void led_disableChannel(TIM_HandleTypeDef &tim, uint8_t chn)
{
    switch (chn)
    {
    case 1U:
        LL_TIM_CC_DisableChannel(tim.Instance, LL_TIM_CHANNEL_CH1);
        break;
    case 2U:
        LL_TIM_CC_DisableChannel(tim.Instance, LL_TIM_CHANNEL_CH2);
        break;
    case 3U:
        LL_TIM_CC_DisableChannel(tim.Instance, LL_TIM_CHANNEL_CH3);
        break;
    case 4U:
        LL_TIM_CC_DisableChannel(tim.Instance, LL_TIM_CHANNEL_CH4);
        break;
    }
}

LED::LED(TIM_HandleTypeDef &tim, TIMER_CHANNEL rChannel, TIMER_CHANNEL gChannel,
         TIMER_CHANNEL bChannel)
    : _tim(tim), _rChannel(rChannel), _gChannel(gChannel),
      _bChannel(bChannel){};

Result LED::start()
{
    // LL_TIM_EnableCounter(ledDevice->tim->Instance);
    led_enableChannel(_tim, _rChannel);
    led_enableChannel(_tim, _gChannel);
    led_enableChannel(_tim, _bChannel);
    return Result_OK;
};

Result LED::stop()
{
    // LL_TIM_DisableCounter(ledDevice->tim->Instance);
    led_disableChannel(_tim, _rChannel);
    led_disableChannel(_tim, _gChannel);
    led_disableChannel(_tim, _bChannel);
    return Result_OK;
};

Result LED::color_set(Color color)
{
    uint32_t ar = LL_TIM_GetAutoReload(_tim.Instance);
    switch (_rChannel)
    {
    case 1U:
        LL_TIM_OC_SetCompareCH1(_tim.Instance, ar * color.r / 256);
        break;
    case 2U:
        LL_TIM_OC_SetCompareCH2(_tim.Instance, ar * color.r / 256);
        break;
    case 3U:
        LL_TIM_OC_SetCompareCH3(_tim.Instance, ar * color.r / 256);
        break;
    case 4U:
        LL_TIM_OC_SetCompareCH4(_tim.Instance, ar * color.r / 256);
        break;
    }

    switch (_gChannel)
    {
    case 1U:
        LL_TIM_OC_SetCompareCH1(_tim.Instance, ar * color.g / 256);
        break;
    case 2U:
        LL_TIM_OC_SetCompareCH2(_tim.Instance, ar * color.g / 256);
        break;
    case 3U:
        LL_TIM_OC_SetCompareCH3(_tim.Instance, ar * color.g / 256);
        break;
    case 4U:
        LL_TIM_OC_SetCompareCH4(_tim.Instance, ar * color.g / 256);
        break;
    }

    switch (_bChannel)
    {
    case 1U:
        LL_TIM_OC_SetCompareCH1(_tim.Instance, ar * color.b / 256);
        break;
    case 2U:
        LL_TIM_OC_SetCompareCH2(_tim.Instance, ar * color.b / 256);
        break;
    case 3U:
        LL_TIM_OC_SetCompareCH3(_tim.Instance, ar * color.b / 256);
        break;
    case 4U:
        LL_TIM_OC_SetCompareCH4(_tim.Instance, ar * color.b / 256);
        break;
    }
    return Result_OK;
};
} // namespace ww::device