//
// Created by zhouj on 2023/2/12.
//

#ifndef WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6835SPI_HPP_
#define WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6835SPI_HPP_

#include "base.hpp"
#include "spi.hpp"
#include "DataSource.hpp"
#include "Crc8.hpp"

namespace wibot
{
    namespace device
    {
        using namespace wibot;
        using namespace wibot::peripheral;

        struct MT6835State
        {
            bool over_speed: 1;
            bool weak_magnet: 1;
            bool over_voltage: 1;
        };

        struct MT6835SpiConfig
        {

        };

        class MT6835SPI : public Initializable,
                          public Configurable<MT6835SpiConfig>,
                          public DataSource
        {
         public:
            MT6835SPI(Spi& spi, EventGroup& eg)
                : spi_(spi), wh_(eg), crc_(0x07, 0x00, 0x00, false, false)
            {
            };

            uint32_t get_angle();

            uint32_t get_data() override;

            MT6835State get_state() const
            {
                return state_;
            }

            uint32_t get_resolution() const
            {
                return 1 << 21;
            }

         protected:
            Result _init() override;
            void _deinit() override;

            Spi& spi_;
            uint8_t buf_[6];
            uint32_t angle_;
            MT6835State state_;
            wibot::WaitHandler wh_;
            Crc8 crc_;
        };

    } // wibot
} // device

#endif //WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6835SPI_HPP_
