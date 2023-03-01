//
// Created by zhouj on 2023/2/28.
//

#include "AS5047SPI.hpp"

namespace wibot
{

    Result AS5047SPI::_init()
    {

        INIT_BEGIN()
        MEMBER_INIT_ERROR_CHECK(spi_);
        spi_.config.autoDisable = true;
        spi_.apply_config();
        INIT_END()

    }

    void AS5047SPI::_deinit()
    {
    }

    uint16_t AS5047SPI::get_angle()
    {
        cmd_ = AS5047_CMD_READ_ANGLECOM;
        spi_.write(&cmd_, 2, wh_);
        wh_.wait(TIMEOUT_FOREVER);
        // cmd_ = AS5047_CMD_READ_ANGLECOM;
        spi_.read(&cmd_, 2, wh_);
        parity_.reset();
        parity_.calculate(static_cast<uint8_t*>(static_cast<void*>(&cmd_)), 2);
        if (parity_.validate())
        {
            angle_ = cmd_ & 0x3FFF;
            if (!(cmd_ & AS5047_EF_BIT))
            {
                state_.invalid_data = true;
            }
            state_.parity_error = false;
        }
        else
        {
            state_.parity_error = true;
        }
        return angle_;
    }
    uint32_t AS5047SPI::get_data()
    {
        return get_angle();
    }
} // wibo
