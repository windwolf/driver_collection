//
// Created by zhouj on 2022/11/16.
//

#include "LinearValueMapper.hpp"

namespace wibot::accessor
{

	float LinearValueMapper::value_get(uint32_t raw_value)
	{
		return (float)(int32_t)(raw_value - config.zero_offset) * config.value_per_unit;
	}

} // wibot::accessor
