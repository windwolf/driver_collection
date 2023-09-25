//
// Created by zhouj on 2023/9/5.
//

#include "ROLA_E22.hpp"


namespace wibot::device {

void ROLA_E22_UART::wait_aux() {
    PinStatus auxStatus;

    // check and ensure AUX is low.
    aux_.read(auxStatus);
    while (auxStatus != PinStatus::Reset) {
        // Utils::delay(1);
        aux_.read(auxStatus);
    }
}

void ROLA_E22_UART::set_mode(ROLA_E22_Mode mode) {


    switch (mode) {
        case ROLA_E22_Mode::Transmition:
            m0_.write(PinStatus::Reset);
            m1_.write(PinStatus::Reset);
            break;
        case ROLA_E22_Mode::WOR:
            m0_.write(PinStatus::Set);
            m1_.write(PinStatus::Reset);
            break;
        case ROLA_E22_Mode::Configuration:
            m0_.write(PinStatus::Reset);
            m1_.write(PinStatus::Set);
            break;
        case ROLA_E22_Mode::Sleep:
            m0_.write(PinStatus::Set);
            m1_.write(PinStatus::Set);
            break;
    }
    mode_ = mode;
}
Result ROLA_E22_UART::_init() {
    config.address  = 0x0000;
    config.net_id = 0x00;
    config.uart_baudrate = ROLA_E22_UART_Baudrate::B9600;
    config.uart_parity = ROLA_E22_UART_Parity::P8N1;
    config.baudrate = ROLA_E22_Baudrate::B2400;
    config.packing = ROLA_E22_Packing::P240;
    config.rssi = ROLA_E22_RSSI::Disable;
    config.rf_power = ROLA_E22_RF_Power::P0;
    config.channel = 0x17;
    config.rssi_byte = ROLA_E22_RSSIBYTE::Disable;
    config.transmition_mode = ROLA_E22_TransmitionMode::Transparent;
    config.relay_mode = ROLA_E22_Relay_Mode::Disable;
    config.lbt_mode = ROLA_E22_LBT_Mode::Disable;
    config.wor_mode = ROLA_E22_WOR_Mode::Receiver;
    config.wor_cycle = ROLA_E22_WOR_CYCLE::C500;
    config.key = 0x00;
    uart_.init();
    wait_aux();
    return Result::OK;
}
void ROLA_E22_UART::_deinit() {
    uart_.deinit();
}
Result ROLA_E22_UART::apply_config() {
    uint8_t cmd[12];
    auto old_mode = mode_;
    wait_aux();
    set_mode(ROLA_E22_Mode::Configuration);
    Utils::delay(1);
    cmd[0] = 0xC0;
    cmd[1] = 0x00;
    cmd[2] = 0x09;
    cmd[3] = (config.address >> 8) & 0xFF;
    cmd[4] = (config.address) & 0xFF;
    cmd[5] = config.net_id;
    cmd[6] = (uint8_t)config.uart_baudrate & (uint8_t)config.uart_parity & (uint8_t)config.baudrate;
    cmd[7] = (uint8_t)config.packing & (uint8_t)config.rssi & (uint8_t)config.rf_power;
    cmd[8] = (config.channel) & 0xFF;
    cmd[9] = (uint8_t)config.rssi_byte & (uint8_t)config.transmition_mode & (uint8_t)config.relay_mode & (uint8_t)config.lbt_mode & (uint8_t)config.wor_mode & (uint8_t)config.wor_cycle;
    cmd[10] = (config.key >> 8) & 0xFF;
    cmd[11] = (config.key) & 0xFF;
    auto waitHandler_ = WaitHandler(events_);
    uart_.write(cmd, 12, waitHandler_);
    waitHandler_.wait(20);
    wait_aux();
    set_mode(old_mode);
    return Configurable::apply_config();
}
void ROLA_E22_UART::send(uint8_t *data, uint8_t len) {
    auto waitHandler_ = WaitHandler(events_);
    uart_.write(data, len, waitHandler_);
    waitHandler_.wait(100);
}
void ROLA_E22_UART::receive(uint8_t *data, uint8_t len) {
    auto waitHandler_ = WaitHandler(events_);
    uart_.read(data, len, waitHandler_);
    waitHandler_.wait(100);
}

}
