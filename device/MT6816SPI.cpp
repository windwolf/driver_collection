//
// Created by zhoujian on 2022/12/12.
//



#include "MT6816SPI.hpp"
#ifdef HAL_SPI_MODULE_ENABLED

#define MT6816_SPI_READ_ANGLE1_REG 0x83
#define MT6816_SPI_READ_ANGLE2_REG 0x84
#define MT6816_SPI_READ_ANGLE3_REG 0x85

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
            MEMBER_INIT_ERROR_CHECK(spi_);
            INIT_END()
        }
        void MT6816SPI::_deinit()
        {
        }
        uint16_t MT6816SPI::get_angle()
        {
            parity_.reset();
            uint16_t angle = 0;
            cmd_[0] = MT6816_SPI_READ_ANGLE1_REG;
            spi_.write_read(cmd_, cmd_, 2, wh_);
            wh_.wait(TIMEOUT_FOREVER);
            parity_.calculate(&cmd_[1], 1);
            angle = cmd_[1] << 6;
            cmd_[0] = MT6816_SPI_READ_ANGLE2_REG;
            spi_.write_read(cmd_, cmd_, 2, wh_);
            wh_.wait(TIMEOUT_FOREVER);
            parity_.calculate(&cmd_[1], 1);
            angle |= cmd_[1] >> 2;
            if (parity_.validate())
            {
                angle_ = angle;
            }
            return angle_;
        }
        
        uint32_t MT6816SPI::get_data()
        {
            return get_angle();
        }
    } // wibot
} // device

#endif
