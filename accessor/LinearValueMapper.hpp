//
// Created by zhouj on 2022/11/16.
//

#ifndef WWMOTOR_LIBS_WWDEVICE_ACCESSOR_VALUEMAPPER_HPP_
#define WWMOTOR_LIBS_WWDEVICE_ACCESSOR_VALUEMAPPER_HPP_

#include "peripheral/adc.hpp"
namespace ww::accessor
{
	using namespace ww::peripheral;

#define VALUE_MAPPER_AXIS_COUNT 3

	struct ValueMapperConfig
	{
		float value_per_unit;
	};

	class ValueMapper : public Configurable<ValueMapperConfig>
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

		uint16_t _range_val[VALUE_MAPPER_AXIS_COUNT];
		float _scale_val[VALUE_MAPPER_AXIS_COUNT];

	};
} // ww::accessor

#endif //WWMOTOR_LIBS_WWDEVICE_ACCESSOR_VALUEMAPPER_HPP_
