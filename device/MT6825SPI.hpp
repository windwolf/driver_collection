//
// Created by zhouj on 2023/1/3.
//

#ifndef WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6825SPI_HPP_
#define WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6825SPI_HPP_
#include "base.hpp"
#include "spi.hpp"
#include "DataSource.hpp"
#ifdef HAL_SPI_MODULE_ENABLED
namespace wibot
{
    namespace device
    {

        using namespace wibot;
        using namespace wibot::peripheral;

        struct MT6825SpiConfig
        {

        };

        class MT6825SPI : public Initializable,
                          public Configurable<MT6825SpiConfig>,
                          public DataSource
        {
         public:
            MT6825SPI(Spi& spi, EventGroup& eg)
                : spi_(spi), wh_(eg)
            {
            };

            uint32_t get_angle();

            uint32_t get_data() override;

         protected:
            Result _init() override;
            void _deinit() override;

            Spi& spi_;
            uint8_t cmd_[4];
            wibot::WaitHandler wh_;
        };

    } // wibot
} // device

#endif

#endif //WWMOTOR_LIBS_WWDEVICE_DEVICE_MT6825SPI_HPP_
