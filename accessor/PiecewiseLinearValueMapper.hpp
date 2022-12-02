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

	struct PiecewiseLinearValueMapperConfig
	{
		float value_per_unit;
	};

	class PiecewiseLinearValueMapper : public ValueMapper, public Configurable<PiecewiseLinearValueMapperConfig>
	{

	 public:

		/**
		 *
		 */
		void calibrate_begin();
		void calibrate_step(uint16_t in_value, float out_value, uint8_t index);
		void calibrate_end();
		/**
		 * 当先验的直到
		 * @param offset
		 */
		void zero_offset_set(uint16_t offset);
		float value_get(uint16_t in_value);

	 private:
		uint16_t _zero_offset;
		uint16_t _last_raw_value;
		uint16_t _last_value;
		// var?

		uint32_t _cal_sum;
		uint8_t _cal_count;

		uint16_t _in_range[VALUE_MAPPER_AXIS_COUNT - 1];
		float _out_range[VALUE_MAPPER_AXIS_COUNT - 1];
		float _range_scale[VALUE_MAPPER_AXIS_COUNT - 1];

	};
} // wibot::accessor

#endif //WWMOTOR_LIBS_WWDEVICE_ACCESSOR_LINEARVALUEMAPPER_HPP_
