//
// Created by zhouj on 2022/11/21.
//

#ifndef WWMOTOR_LIBS_WWDEVICE_ACCESSOR_VALUEMAPPER_HPP_
#define WWMOTOR_LIBS_WWDEVICE_ACCESSOR_VALUEMAPPER_HPP_
#include "base.hpp"
namespace ww::accessor
{

	class ValueMapper
	{
	 public:
		virtual float value_get(uint32_t in_value) = 0;
	};

} // wwMotor2

#endif //WWMOTOR_LIBS_WWDEVICE_ACCESSOR_VALUEMAPPER_HPP_
