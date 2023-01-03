//
// Created by zhouj on 2022/12/12.
//

#include "MT6816SPI.hpp"
#ifdef HAL_SPI_MODULE_ENABLED

#define MT6816_SPI_READ_CMD 0x80
#define MT6816_SPI_WRITE_CMD 0x00
#define MT6816_SPI_ANGLE1_REG 0x03
#define MT6816_SPI_ANGLE2_REG 0x04
#define MT6816_SPI_ANGLE3_REG 0x05
#define MT6816_SPI_ANGLE_BIT_MASK 0xF8
#define MT6816_SPI_NO_MAG_BIT_MASK 0x02
#define MT6816_SPI_PC_BIT_MASK 0x01
#define MT6816_SPI_OVER_SPEED_BIT_MASK 0x18

namespace wibot
{
    namespace device
    {
        Result MT6816SPI::_init()
        {
            INIT_BEGIN()
            scope_ = wh_.scope_begin();
            MEMBER_INIT_ERROR_CHECK(spi_);
            INIT_END()
        }
        void MT6816SPI::_deinit()
        {
            wh_.scope_end();
        }
        uint16_t MT6816SPI::get_angle()
        {
            cmd_[0] = MT6816_SPI_ANGLE1_REG | MT6816_SPI_READ_CMD;
            spi_.write_read(cmd_, cmd_, 4, wh_);
            wh_.wait(scope_, TIMEOUT_FOREVER);
            return (cmd_[1] << 6) | (cmd_[2] >> 2);
        }
        uint32_t MT6816SPI::get_data()
        {
            return get_angle();
        }
    } // wibot
} // device

#endif
