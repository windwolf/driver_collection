//
// Created by zhouj on 2022/12/12.
//

#ifndef WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6816_SPI_HPP_
#define WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6816_SPI_HPP_

#include "base.hpp"
#include "peripheral.hpp"

#ifdef HAL_SPI_MODULE_ENABLED

namespace wibot::device
{

	struct MT6816SpiConfig
	{
	};

	class MT6816_SPI : public Initializable, public Configurable<MT6816SpiConfig>
	{
	 public:
	 protected:
		Result _init() override;
		void _deinit() override;

	};

} // wibot.::device

#endif

#endif //WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6816_SPI_HPP_
