//
// Created by zhouj on 2022/12/2.
//

#include "AS5600I2C.hpp"
#include "misc.hpp"
void wibot::device::AS5600I2C::zero_set()
{
    const uint16_t calibrationRound = 500;
    uint8_t data[2] = { 0x00, 0x00 };

    uint32_t pos_sum = 0;
    for (int i = 0; i < calibrationRound; i++)
    {
        _i2c.read(AS5600_I2C_RAWANGLE, data, 2, _wh);
        _wh.wait(TIMEOUT_FOREVER);
        uint16_t pos = (data[0] << 8) | data[1];
        pos_sum += pos;
        Misc::ms_delay(1);
    }
    pos_sum /= calibrationRound;
    data[0] = (pos_sum >> 8) & 0xFF;
    data[1] = pos_sum & 0xFF;
    _i2c.write(AS5600_I2C_ZPOS, data, 2, _wh);
    _wh.wait(TIMEOUT_FOREVER);
    _i2c.write(AS5600_I2C_MPOS, data, 2, _wh);
    _wh.wait(TIMEOUT_FOREVER);
    _i2c.read(AS5600_I2C_ANGLE, data, 2, _wh);
    _wh.wait(TIMEOUT_FOREVER);
}

Result wibot::device::AS5600I2C::_init()
{
    INIT_BEGIN()
    MEMBER_INIT_ERROR_CHECK(_i2c)
    apply_config();
    INIT_END()
}

uint32_t wibot::device::AS5600I2C::angle_get()
{
    uint8_t data[2] = { 0x00, 0x00 };
    _i2c.read(AS5600_I2C_ANGLE, data, 2, _wh);
    _wh.wait(TIMEOUT_FOREVER);
    return (data[0] << 8) | data[1];
}

Result wibot::device::AS5600I2C::apply_config()
{
    _i2c.config.slaveAddress = AS5600_I2C_ADDRESS << 1;
    _i2c.config.dataWidth = DataWidth::Bit8;
    return _i2c.apply_config();
}
void wibot::device::AS5600I2C::_deinit()
{
    MEMBER_DEINIT(_i2c)
}
uint32_t wibot::device::AS5600I2C::get_data()
{
    return angle_get();
}
uint16_t wibot::device::AS5600I2C::get_config()
{
    uint8_t data[2] = { 0x00, 0x00 };
    _i2c.read(AS5600_I2C_CONF, data, 2, _wh);
    _wh.wait(TIMEOUT_FOREVER);
    return (data[0] << 8) | data[1];
}
uint8_t wibot::device::AS5600I2C::get_status()
{
    uint8_t data[1] = { 0x00 };
    _i2c.read(AS5600_I2C_STATUS, data, 1, _wh);
    _wh.wait(TIMEOUT_FOREVER);
    return data[0];
}
uint16_t wibot::device::AS5600I2C::get_zpos()
{
    uint8_t data[2] = { 0x00, 0x00 };
    _i2c.read(AS5600_I2C_ZPOS, data, 2, _wh);
    _wh.wait(TIMEOUT_FOREVER);
    return (data[0] << 8) | data[1];
}
uint16_t wibot::device::AS5600I2C::get_mpos()
{
    uint8_t data[2] = { 0x00, 0x00 };
    _i2c.read(AS5600_I2C_MPOS, data, 2, _wh);
    _wh.wait(TIMEOUT_FOREVER);
    return (data[0] << 8) | data[1];
}
