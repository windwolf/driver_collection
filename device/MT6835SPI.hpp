//
// Created by zhouj on 2023/2/12.
//

#ifndef WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6835SPI_HPP_
#define WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6835SPI_HPP_

#include "base.hpp"
#include "spi.hpp"
#include "DataSource.hpp"

namespace wibot
{
    namespace device
    {
        using namespace wibot;
        using namespace wibot::peripheral;

        struct MT6835SpiConfig
        {

        };

        class MT6835SPI : public Initializable,
                          public Configurable<MT6835SpiConfig>,
                          public DataSource
        {
         public:
            MT6835SPI(Spi& spi, EventGroup& eg)
                : spi_(spi), wh_(eg)
            {
            };

            uint32_t get_angle();

            uint32_t get_data() override;

         protected:
            Result _init() override;
            void _deinit() override;

            Spi& spi_;
            uint8_t cmd_[6];
            wibot::WaitHandler wh_;
        };

    } // wibot
} // device

#endif //WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6835SPI_HPP_
