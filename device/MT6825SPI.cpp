//
// Created by zhouj on 2023/1/3.
//

#include "MT6825SPI.hpp"
#ifdef HAL_SPI_MODULE_ENABLED

#define MT6825_SPI_READ_CMD 0x80
#define MT6825_SPI_WRITE_CMD 0x00
#define MT6825_SPI_ANGLE1_REG 0x03
#define MT6825_SPI_ANGLE2_REG 0x04
#define MT6825_SPI_ANGLE3_REG 0x05

#define MT6825_SPI_ANGLE_BIT_MASK 0xF8
#define MT6825_SPI_NO_MAG_BIT_MASK 0x02
#define MT6825_SPI_PC_BIT_MASK 0x01
#define MT6825_SPI_OVER_SPEED_BIT_MASK 0x18

namespace wibot
{
    namespace device
    {
        Result MT6825SPI::_init()
        {
            INIT_BEGIN()
            MEMBER_INIT_ERROR_CHECK(spi_);
            INIT_END()
        }
        void MT6825SPI::_deinit()
        {
        }
        uint32_t MT6825SPI::get_angle()
        {
            cmd_[0] = MT6825_SPI_ANGLE1_REG | MT6825_SPI_READ_CMD;
            spi_.write_read(cmd_, cmd_, 4, wh_);
            wh_.wait(scope_);
            return (cmd_[1] << 10) | ((cmd_[2] & 0xfc) << 2) | (cmd_[3] >> 4);
        }
        uint32_t MT6825SPI::get_data()
        {
            return get_angle();
        }
    } // wibot
} // device

#endif
