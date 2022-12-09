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
		_wh.wait(_scope, TIMEOUT_FOREVER);
		uint16_t pos = (data[0] << 8) | data[1];
		pos_sum += pos;
		Misc::ms_delay(1);
	}
	pos_sum /= calibrationRound;
	data[0] = pos_sum & 0xFF;
	data[1] = (pos_sum >> 8) & 0xFF;
	_i2c.write(AS5600_I2C_ZPOS, data, 2, _wh);
	_wh.wait(_scope, TIMEOUT_FOREVER);
	_i2c.write(AS5600_I2C_MPOS, data, 2, _wh);
	_wh.wait(_scope, TIMEOUT_FOREVER);
	_i2c.read(AS5600_I2C_ANGLE, data, 2, _wh);
	_wh.wait(_scope, TIMEOUT_FOREVER);
}

Result wibot::device::AS5600I2C::_init()
{
	INIT_BEGIN()
	MEMBER_INIT_ERROR_CHECK(_i2c)
	auto& cfg = _i2c.config;
	cfg.slaveAddress = AS5600_I2C_ADDRESS << 1;
	cfg.dataWidth = DataWidth::Bit8;
	cfg.useRxDma = false;
	cfg.useTxDma = false;
	_scope = _wh.scope_begin();
	INIT_END()
}

void wibot::device::AS5600I2C::angle_get(uint32_t& angle)
{
	uint8_t data[2] = { 0x00, 0x00 };
	_i2c.read(AS5600_I2C_ANGLE, data, 2, _wh);
	_wh.wait(_scope, TIMEOUT_FOREVER);
	angle = (data[0] << 8) | data[1];
}

void wibot::device::AS5600I2C::config_apply(wibot::device::AS5600I2CConfig& config)
{
	this->config = config;
	_i2c.config.slaveAddress = AS5600_I2C_ADDRESS << 1;
}
void wibot::device::AS5600I2C::_deinit()
{
	MEMBER_DEINIT(_i2c)
	_wh.scope_end();

}
