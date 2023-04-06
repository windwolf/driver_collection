#ifndef __WWDEVICE_RX8010_HPP__
#define __WWDEVICE_RX8010_HPP__

#include "os.hpp"
#include "soc.hpp"
#include "temporal/temporal.hpp"

namespace wibot::device {
using namespace wibot::peripheral;
using namespace wibot::os;
using namespace wibot::temporal;

class RX8010 : public Initializable {
   public:
    RX8010(I2cMaster &i2c, EventGroup &eventGroup);
    Result _init() override;
    void   _deinit() override;
    Result por_init();
    Result datetime_get(DateTime &datetime);
    Result datetime_set(const DateTime &datetime);

   private:
    I2cMaster  &_i2c;
    WaitHandler _waitHandler;
    Result      _i2c_read(uint32_t address, void *data, uint32_t dataSize);
    Result      _i2c_write(uint32_t address, void *data, uint32_t dataSize);
};

}  // namespace wibot::device

#endif  // __WWDEVICE_RX8010_HPP__
