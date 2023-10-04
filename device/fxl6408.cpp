//
// Created by zhouj on 2023/9/27.
//

#include "fxl6408.hpp"

namespace wibot {
namespace device {
Fxl6408_SoftI2c::Fxl6408_SoftI2c(SoftI2cMaster& i2c): _i2c(i2c) {

}
Result Fxl6408_SoftI2c::_init() {
    _i2c.init();
    _i2c.config.slaveAddress = 0b1000011;
    _i2c.config.dataWidth = DataWidth::Bit8;
    _i2c.apply_config();

    return Result::OK;
}
void Fxl6408_SoftI2c::_deinit() {
    _i2c.deinit();
}
void Fxl6408_SoftI2c::reset() {
    uint8_t ids;
    uint8_t addr;
    addr = 0x01,
    _i2c.writeRead(&addr,1, &ids, 1);
    ids |= 0x01;
    _i2c.writeWrite(&addr,1, &ids, 1);

}; uint8_t Fxl6408_SoftI2c::getId() {
    uint8_t ids;
    uint8_t addr;
    addr = 0x01,
    _i2c.writeRead(&addr,1, &ids, 1);
    return ids;
}
void Fxl6408_SoftI2c::setDirection(Fxl6408Pins pins, bool isOutput){
    uint8_t v;
    uint8_t addr;
    addr = 0x03,
    _i2c.writeRead(&addr,1, &v, 1);
    if (isOutput)
    {
        v |= pins;
    }
    else {
        v &= ~pins;
    }
    _i2c.writeWrite(&addr,1, &v, 1);
}
void Fxl6408_SoftI2c::setOutputs(Fxl6408Pins pins, Fxl6408Pins values, Fxl6408Pins highZ){

    uint8_t o;
    uint8_t h;
    uint8_t addr;
    addr = 0x05,
    _i2c.writeRead(&addr,1, &o, 1);
    addr = 0x07,
    _i2c.writeRead(&addr,1, &h, 1);

    h &= ~pins;
    h |= pins & highZ;
    _i2c.writeWrite(&addr,1, &h, 1);

    o &= ~pins;
    o |= pins & values;
    _i2c.writeWrite(&addr,1, &o, 1);

}
Fxl6408Pins Fxl6408_SoftI2c::getInputs(Fxl6408Pins pins) {
    uint8_t i;
    uint8_t addr;
    addr = 0x0F,
    _i2c.writeRead(&addr,1, &i, 1);
    return i & pins;
}
Fxl6408Pins Fxl6408_SoftI2c::getInterrupts(Fxl6408Pins pins) {
    uint8_t i;
    uint8_t addr;
    addr = 0x13,
    _i2c.writeRead(&addr,1, &i, 1);
    return i & pins;
}
void Fxl6408_SoftI2c::enablePull(Fxl6408Pins pins, bool isUp){
    uint8_t e;
    uint8_t u;
    uint8_t addr;
    addr = 0x0B,
    _i2c.writeRead(&addr,1, &e, 1);
    e |= pins;
    _i2c.writeWrite(&addr,1, &e, 1);
    addr = 0x0D,
    _i2c.writeRead(&addr,1, &u, 1);
    if (isUp) {
        u |= pins;
    }
    else {
        u &= ~pins;
    }
    _i2c.writeWrite(&addr,1, &u, 1);
}
void Fxl6408_SoftI2c::disablePull(Fxl6408Pins pins){
    uint8_t e;
    uint8_t addr;
    addr = 0x0B,
    _i2c.writeRead(&addr,1, &e, 1);
    e &= ~pins;
    _i2c.writeWrite(&addr,1, &e, 1);
}
void Fxl6408_SoftI2c::configInterruptMask(Fxl6408Pins masks){
    uint8_t addr;
    addr = 0x11,
    _i2c.writeWrite(&addr,1, &masks, 1);

}
void Fxl6408_SoftI2c::configInputDefault(Fxl6408Pins values){
    uint8_t addr;
    addr = 0x09,
    _i2c.writeWrite(&addr,1, &values, 1);
};
}  // namespace device
}  // namespace wibot
