//
// Created by zhouj on 2023/2/12.
//

#include "MT6835SPI.hpp"

#define MT6835SPI_CONT_READ_CMD1 0xA0
#define MT6835SPI_CONT_READ_CMD2 0x03

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
            buf_[0] = MT6835SPI_CONT_READ_CMD1;
            buf_[1] = MT6835SPI_CONT_READ_CMD2;
            spi_.write_read(buf_, buf_, 6, wh_);
            wh_.wait(TIMEOUT_FOREVER);
            crc_.reset();
            crc_.calculate(&buf_[2], 4);
            if (crc_.validate(buf_[5]))
            {
                angle_ = (buf_[2] << 13) | (buf_[3] << 5) | (buf_[4] >> 3);
                state_.over_speed = buf_[4] & 0x01;
                state_.weak_magnet = buf_[4] & 0x02;
                state_.over_voltage = buf_[4] & 0x04;
            }
            return angle_;
        }
        uint32_t MT6835SPI::get_data()
        {
            return get_angle();
        }
    } // wibot
} // device
