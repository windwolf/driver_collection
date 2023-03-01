//
// Created by zhouj on 2022/12/12.
//

#ifndef WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6816SPI_HPP_
#define WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6816SPI_HPP_

#include "base.hpp"
#include "spi.hpp"
#include "DataSource.hpp"
#include "ParityCalculator.hpp"

#ifdef HAL_SPI_MODULE_ENABLED

namespace wibot::device
{
    using namespace wibot;
    using namespace wibot::peripheral;

    struct MT6816SpiConfig
    {

    };

    class MT6816SPI : public Initializable,
                      public Configurable<MT6816SpiConfig>,
                      public DataSource
    {
     public:
        MT6816SPI(Spi& spi, EventGroup& eg)
            : spi_(spi), wh_(eg), parity_(true)
        {
        };

        uint16_t get_angle();

        uint32_t get_data() override;

     protected:
        Result _init() override;
        void _deinit() override;

        Spi& spi_;
        uint8_t cmd_[4];
        wibot::WaitHandler wh_;
        uint16_t angle_;
        ParityCalculator parity_;
    };

} // wibot.::device

#endif //HAL_SPI_MODULE_ENABLED

#endif //WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6816SPI_HPP_
