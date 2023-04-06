//
// Created by zhouj on 2023/2/28.
//

#include "AS5047SPI.hpp"

namespace wibot {

Result AS5047SPI::_init() {
    INIT_BEGIN()
    MEMBER_INIT_ERROR_CHECK(spi_);
    spi_.config.autoDisable = false;
    spi_.apply_config();
    INIT_END()
}

void AS5047SPI::_deinit() {
}

uint16_t AS5047SPI::get_angle() {
    cmd_[0] = AS5047_CMD_READ_ANGLECOM;
    cmd_[1] = AS5047_CMD_READ_ANGLECOM;
    spi_.write_read(&cmd_, cmd_, 4, wh_);
    parity_.reset();
    wh_.wait(TIMEOUT_FOREVER);
    parity_.calculate(static_cast<uint8_t*>(static_cast<void*>(&cmd_[1])), 2);
    if (parity_.validate(nullptr)) {
        angle_ = cmd_[1] & 0x3FFF;
        if (!(cmd_[1] & AS5047_EF_BIT)) {
            state_.invalid_data = true;
        }
        state_.parity_error = false;
    } else {
        state_.parity_error = true;
    }
    return angle_;
}
uint32_t AS5047SPI::get_data() {
    return get_angle();
}
}  // namespace wibot
