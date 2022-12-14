//
// Created by zhouj on 2022/12/12.
//

#include "MT6816_SPI.hpp"
#ifdef HAL_SPI_MODULE_ENABLED
namespace wibot
{
	namespace device
	{
		Result MT6816_SPI::_init()
		{
			return Result::Timeout;
		}
		void MT6816_SPI::_deinit()
		{

		}
	} // wibot
} // device

#endif
