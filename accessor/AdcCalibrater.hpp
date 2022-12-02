//
// Created by zhouj on 2022/11/21.
//

#ifndef WWMOTOR_LIBS_WWDEVICE_ACCESSOR_ADCCALEBRATER_HPP_
#define WWMOTOR_LIBS_WWDEVICE_ACCESSOR_ADCCALEBRATER_HPP_

#include "adc.hpp"
namespace ww::accessor
{
	using namespace ww::peripheral;
	struct AdcCalebraterResult
	{
		uint16_t zero_offset;
		uint16_t variance;
	};
	class AdcCalibrater
	{
	 public:
		void calibrate_begin(Adc* adc);
		void calibrate_step();
		void calibrate_begin(AdcCalebraterResult& result);

	};

} // ww::accessor

#endif //WWMOTOR_LIBS_WWDEVICE_ACCESSOR_ADCCALEBRATER_HPP_
