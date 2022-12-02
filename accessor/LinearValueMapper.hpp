//
// Created by zhouj on 2022/11/16.
//

#ifndef WWMOTOR_LIBS_WWDEVICE_ACCESSOR_LINEARVALUEMAPPER_HPP_
#define WWMOTOR_LIBS_WWDEVICE_ACCESSOR_LINEARVALUEMAPPER_HPP_

#include "base.hpp"
#include "ValueMapper.hpp"

namespace wibot::accessor
{

#define VALUE_MAPPER_AXIS_COUNT 3

	struct LinearValueMapperConfig
	{
		uint32_t zero_offset;
		float value_per_unit;
	};

	class LinearValueMapper : public ValueMapper, public Configurable<LinearValueMapperConfig>
	{

	 public:

		float value_get(uint32_t in_value);

	 private:
	};
} // wibot::accessor

#endif //WWMOTOR_LIBS_WWDEVICE_ACCESSOR_LINEARVALUEMAPPER_HPP_
