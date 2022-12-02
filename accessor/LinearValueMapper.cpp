//
// Created by zhouj on 2022/11/16.
//

#include "LinearValueMapper.hpp"

namespace ww::accessor
{

	float LinearValueMapper::value_get(uint32_t raw_value)
	{
		return (float)(raw_value - _config.zero_offset) * _config.value_per_unit;
	}

} // ww::accessor
