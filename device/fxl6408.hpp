//
// Created by zhouj on 2023/9/27.
//

#ifndef AQ_SPXX_RC_LIBS_WIBOTDEVICE_DEVICE_FXL6408_HPP_
#define AQ_SPXX_RC_LIBS_WIBOTDEVICE_DEVICE_FXL6408_HPP_

#include "i2c.hpp"
namespace wibot::device {

using namespace wibot;
using namespace wibot::peripheral;
using namespace wibot::os;


using Fxl6408Pins = uint8_t;
constexpr Fxl6408Pins Pin0 = 0x01;
constexpr Fxl6408Pins Pin1 = 0x02;
constexpr Fxl6408Pins Pin2 = 0x04;
constexpr Fxl6408Pins Pin3 = 0x08;
constexpr Fxl6408Pins Pin4 = 0x10;
constexpr Fxl6408Pins Pin5 = 0x20;
constexpr Fxl6408Pins Pin6 = 0x40;
constexpr Fxl6408Pins Pin7 = 0x80;
constexpr Fxl6408Pins PinAll = 0xFF;
constexpr Fxl6408Pins PinNone = 0x00;
class Fxl6408_SoftI2c : Initializable {
   public:
    explicit Fxl6408_SoftI2c(SoftI2cMaster& i2c);



    void reset();

    uint8_t getId();


    void setDirection(Fxl6408Pins pins, bool isOutput = true);

    void setOutputs(Fxl6408Pins pins, Fxl6408Pins values, Fxl6408Pins highZ);

    Fxl6408Pins getInputs(Fxl6408Pins pins);

    Fxl6408Pins getInterrupts(Fxl6408Pins pins);

    /**
     *
     * @param enables
     * @param pullDownOrUp 0=down, 1=up
     */
    void enablePull(Fxl6408Pins pins, bool isUp = true);
    void disablePull(Fxl6408Pins pins);
    void configInterruptMask(Fxl6408Pins masks);
    
    void configInputDefault(Fxl6408Pins values);

   private:
    Result _init() override;
    void   _deinit() override;

   private:
    SoftI2cMaster& _i2c;

};

}  // namespace wibot

#endif  //AQ_SPXX_RC_LIBS_WIBOTDEVICE_DEVICE_FXL6408_HPP_
