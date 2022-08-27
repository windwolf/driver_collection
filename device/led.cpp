#include "led.hpp"

namespace ww::device
{

Result LED::start()
{
    _pwm.config_get() = PwmConfig{
        .channelsEnable = (_rChannel | _gChannel | _bChannel),
        .fullScaleDuty = 256,
    };
    _pwm.init();
    return _pwm.start();
}; // namespace ww::device

Result LED::stop()
{
    return _pwm.stop();
};

Result LED::color_set(Color color)
{
    _pwm.duty_set(_rChannel, color.r);
    _pwm.duty_set(_gChannel, color.g);
    _pwm.duty_set(_bChannel, color.b);

    return Result_OK;
};
} // namespace ww::device