#ifndef __WWDEVICE_RX8010_HPP__
#define __WWDEVICE_RX8010_HPP__

#include "os.hpp"
#include "soc.hpp"
#include "temporal/temporal.hpp"

namespace ww::device
{
using namespace ww::peripheral;
using namespace ww::os;
using namespace ww::temporal;

class RX8010 : public Initializable
{

  public:
    RX8010(I2cMaster &i2c, EventGroup &eventGroup, uint32_t doneFlag,
           uint32_t errorFlag);
    Result init();
    Result datetime_get(DateTime &datetime);
    Result datetime_set(const DateTime &datetime);

  private:
    I2cMaster &_i2c;
    EventGroupWaitHandler _waitHandler;

    Result _i2c_read(uint32_t address, void *data, uint32_t dataSize);
    Result _i2c_write(uint32_t address, void *data, uint32_t dataSize);
};

} // namespace ww::device

#endif // __WWDEVICE_RX8010_HPP__