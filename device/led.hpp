#ifndef __WWDEVICE_LED_HPP__
#define __WWDEVICE_LED_HPP__

#include "graph.hpp"
#include "os.hpp"
#include "soc.hpp"

namespace ww::device
{
using namespace ww::graph;
#define TIMER_CHANNEL uint8_t
class LED
{
  public:
    LED(TIM_HandleTypeDef &tim, TIMER_CHANNEL rChannel, TIMER_CHANNEL gChannel,
        TIMER_CHANNEL bChannel);
    Result start();
    Result stop();
    Result color_set(Color color);

  private:
    TIM_HandleTypeDef &_tim;
    TIMER_CHANNEL _rChannel;
    TIMER_CHANNEL _gChannel;
    TIMER_CHANNEL _bChannel;
};
} // namespace ww::device

#endif // __WWDEVICE_LED_HPP__