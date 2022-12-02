//
// Created by zhouj on 2022/11/16.
//

#include "ValueMapper.hpp"

namespace ww::accessor
{
	void ValueMapper::zero_offset_set(uint16_t offset)
	{
		_zero_offset = offset;
	}
	float ValueMapper::value_get(uint16_t raw_value)
	{
		return (float)(raw_value - _zero_offset) * _config.value_per_unit;
	}
	void ValueMapper::calibrate_begin()
	{
		_cal_sum = 0;
		_cal_count = 0;
	}
	void ValueMapper::calibrate_step(uint16_t in_value, float out_value, uint8_t index)
	{
		_range_val[index] = in_value;
		_scale_val[index] = out_value;
	}
	void ValueMapper::calibrate_end()
	{
		_zero_offset = _cal_sum / _cal_count;
	}
} // ww::accessor
