//
// Created by zhouj on 2023/2/12.
//

#include "MT6835SPI.hpp"

#define MT6835SPI_READ_CMD 0xA0
#define MT6835SPI_ANGLE_REG 0x03

namespace wibot
{
    namespace device
    {
        Result MT6835SPI::_init()
        {
            INIT_BEGIN()
            MEMBER_INIT_ERROR_CHECK(spi_);
            INIT_END()
        }
        void MT6835SPI::_deinit()
        {
        }
        uint32_t MT6835SPI::get_angle()
        {
            uint8_t data[6];
            cmd_[0] = MT6835SPI_READ_CMD;
            cmd_[1] = MT6835SPI_ANGLE_REG;
            spi_.write_read(cmd_, data, 6, wh_);
            wh_.wait(TIMEOUT_FOREVER);
            return (data[2] << 13) | (data[3] << 5) | (data[4] >> 3);
        }
        uint32_t MT6835SPI::get_data()
        {
            return get_angle();
        }
    } // wibot
} // device
