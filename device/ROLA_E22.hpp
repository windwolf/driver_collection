//
// Created by zhouj on 2023/9/5.
//

#ifndef AQ_SPXX_RC_LIBS_WIBOTDEVICE_DEVICE_ROLA_E22_HPP_
#define AQ_SPXX_RC_LIBS_WIBOTDEVICE_DEVICE_ROLA_E22_HPP_


#include "base.hpp"
#include "uart.hpp"
#include "pin.hpp"



namespace wibot::device {

using namespace wibot::peripheral;

enum class ROLA_E22_Mode {
    Transmition,
    WOR,
    Configuration,
    Sleep,
};

enum class ROLA_E22_UART_Baudrate : uint8_t {
    B1200 = 0x00,
    B2400 = 0x20,
    B4800 = 0x40,
    B9600 = 0x60,
    B19200 = 0x80,
    B38400 = 0xA0,
    B57600 = 0xC0,
    B115200 = 0xE0,
};

enum class ROLA_E22_UART_Parity : uint8_t {
    P8N1 = 0x00,
    P8O1 = 0x08,
    P8E1 = 0x10,
};

enum class ROLA_E22_Baudrate : uint8_t {
    B2400 = 0x02,
    B4800 = 0x03,
    B9600 = 0x04,
    B19200 = 0x05,
    B38400 = 0x06,
    B57600 = 0x07,

};

enum class ROLA_E22_Packing : uint8_t {
    P240 = 0x00,
    P128 = 0x40,
    P64 = 0x80,
    P32 = 0xC0,
};

enum class ROLA_E22_RSSI : uint8_t {
    Disable = 0x00,
    Enable = 0x20,
};

enum class ROLA_E22_RF_Power : uint8_t {
    P0 = 0x00, // 22/30/33dbm
    P1 = 0x01, // 17/27/30dbm
    P2 = 0x02, // 14/24/27dbm
    P3 = 0x03, // 11/21/24dbm
};

enum class ROLA_E22_RSSIBYTE : uint8_t {
    Disable = 0x00,
    Enable = 0x80,
};

enum class ROLA_E22_TransmitionMode : uint8_t {
    Transparent = 0x00,
    Fixedport = 0x40,
};

enum class ROLA_E22_Relay_Mode : uint8_t {
    Disable = 0x00,
    Enable = 0x20,
};

enum class ROLA_E22_LBT_Mode : uint8_t {
    Disable = 0x00,
    Enable = 0x10,
};

enum class ROLA_E22_WOR_Mode : uint8_t {
    Receiver = 0x00,
    Transmitter = 0x08,
};

enum class ROLA_E22_WOR_CYCLE : uint8_t {
    C500 = 0x00,
    C1000 = 0x01,
    C1500 = 0x02,
    C2000 = 0x03,
    C2500 = 0x04,
    C3000 = 0x05,
    C3500 = 0x06,
    C4000 = 0x07,
};




struct ROLA_E22_Config {
    uint8_t net_id;
    uint16_t address;
    uint16_t channel; // 410.125MHz + 1MHz * ch
    ROLA_E22_UART_Baudrate uart_baudrate;
    ROLA_E22_UART_Parity uart_parity;
    ROLA_E22_Baudrate baudrate;
    ROLA_E22_Packing packing;
    ROLA_E22_RSSI rssi;
    ROLA_E22_RF_Power rf_power;
    ROLA_E22_RSSIBYTE rssi_byte;
    ROLA_E22_TransmitionMode transmition_mode;
    ROLA_E22_Relay_Mode relay_mode;
    ROLA_E22_LBT_Mode lbt_mode;
    ROLA_E22_WOR_Mode wor_mode;
    ROLA_E22_WOR_CYCLE wor_cycle;

    uint16_t key;
};


/**
 * @brief ROLA-E22
 * @note Fixedport: transmitionMode=fixedport, data[0]=ADDRH, data[1]=ADDRL, data[2]=CH,
 * @note Broadcast: transmitionMode=transparent,
 * @note Listen: ADDRH=0xFF, ADDRL=0xFF,
 */
    class ROLA_E22_UART: public Initializable, public Configurable<ROLA_E22_Config>  {
       public:
        ROLA_E22_UART(UART& uart, Pin& m0, Pin& m1, Pin& aux, EventGroup& events)
            : uart_(uart),
              m0_(m0),
              m1_(m1),
              aux_(aux),events_(events) {

        }

        void send(uint8_t* data, uint8_t len);

        void receive(uint8_t* data, uint8_t len);

        Result apply_config() override;

        void set_mode(ROLA_E22_Mode mode);
        void wait_aux();
       protected:

        Result _init() override;
        void   _deinit() override;

       private:
        UART& uart_;
        Pin& m0_;
        Pin& m1_;
        Pin& aux_;
        EventGroup& events_;
        ROLA_E22_Mode mode_;

    };
}
#endif  //AQ_SPXX_RC_LIBS_WIBOTDEVICE_DEVICE_ROLA_E22_HPP_
