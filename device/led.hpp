#ifndef __WWDEVICE_LED_HPP__
#define __WWDEVICE_LED_HPP__

#include "graph.hpp"
#include "os.hpp"
#include "soc.hpp"

namespace wibot::device {
using namespace wibot::graph;
using namespace wibot::peripheral;
#define TIMER_CHANNEL uint8_t

class LED : public Initializable {
   public:
    LED(Pwm &pwm, PwmChannel rChannel, PwmChannel gChannel, PwmChannel bChannel);
    Result _init() override;
    void   _deinit() override;
    Result start();
    Result stop();
    Result color_set(Color color);

   private:
    Pwm       &_pwm;
    PwmChannel _rChannel;
    PwmChannel _gChannel;
    PwmChannel _bChannel;
};
}  // namespace wibot::device

#endif  // __WWDEVICE_LED_HPP__
