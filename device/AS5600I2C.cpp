//
// Created by zhouj on 2022/12/2.
//

#include "AS5600I2C.hpp"
void wi::device::AS5600I2C::zero_set()
{
	auto lv = _wh.scope_begin();
	uint8_t data[2] = { 0x00, 0x00 };

	// test begin
	_i2c.read(AS5600_I2C_ZPOS, data, 2, _wh);
	_wh.wait(lv, TIMEOUT_FOREVER);
	_i2c.read(AS5600_I2C_MPOS, data, 2, _wh);
	_wh.wait(lv, TIMEOUT_FOREVER);
	// test end
	_i2c.read(AS5600_I2C_RAWANGLE, data, 2, _wh);
	_wh.wait(lv, TIMEOUT_FOREVER);
	_i2c.write(AS5600_I2C_ZPOS, data, 2, _wh);
	_wh.wait(lv, TIMEOUT_FOREVER);
	_i2c.write(AS5600_I2C_MPOS, data, 2, _wh);
	_wh.wait(lv, TIMEOUT_FOREVER);
	_wh.scope_end();
}

Result wi::device::AS5600I2C::_init()
{
	auto& cfg = _i2c.config_get();
	cfg.slaveAddress = AS5600_I2C_ADDRESS;
	cfg.dataWidth = DataWidth::Bit8;
	cfg.useRxDma = false;
	cfg.useTxDma = false;
	return _i2c.init();
}

void wi::device::AS5600I2C::angle_get(uint32_t& angle)
{
	auto lv = _wh.scope_begin();
	uint8_t data[2] = { 0x00, 0x00 };
	_i2c.read(AS5600_I2C_ANGLE, data, 2, _wh);
	_wh.wait(lv, TIMEOUT_FOREVER);
	angle = (data[0] << 8) | data[1];
	_wh.scope_end();
}
void wi::device::AS5600I2C::config_apply(wi::device::AS5600I2CConfig& config)
{
	Configurable::config_apply(config);
}
void wi::device::AS5600I2C::_deinit()
{
	_i2c.deinit();

}
