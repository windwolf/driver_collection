//
// Created by zhouj on 2023/2/28.
//

#ifndef WWMOTOR_LIBS_WWDEVICE_DEVICE_AS5047SPI_HPP_
#define WWMOTOR_LIBS_WWDEVICE_DEVICE_AS5047SPI_HPP_

#include "DataSource.hpp"
#include "Parity.hpp"
#include "base.hpp"
#include "spi.hpp"

#define AS5047_EF_BIT     0x4000
#define AS5047_PARITY_BIT 0x8000

#define AS5047_CMD_READ_MAG      0x7FFD  // 0x3FFD | AS5047_EF_BIT | 0x0000
#define AS5047_CMD_READ_ANGLEUNC 0x7FFE  // 0x3FFE | AS5047_EF_BIT | 0x0000
#define AS5047_CMD_READ_ANGLECOM 0xFFFF  // 0x3FFF | AS5047_EF_BIT | 0x8000

namespace wibot {
using namespace wibot;
using namespace wibot::peripheral;

struct AS5047SPIConfig {};

struct AS5047State {
    bool parity_error  : 1;
    bool framing_error : 1;
    bool command_error : 1;
    bool invalid_data  : 1;
};

/**
 * @note 使用前，确保时钟极性为极性为0，下降沿采样，16位，NSS脉冲模式
 */
class AS5047SPI : public Initializable, public Configurable<AS5047SPIConfig>, public DataSource {
   public:
    AS5047SPI(Spi& spi, EventGroup& eg)
        : spi_(spi), wh_(eg), parity_(true){

                              };

    uint16_t get_angle();

    uint32_t get_data() override;

    AS5047State get_state() const {
        return state_;
    }

   protected:
    Result _init() override;
    void   _deinit() override;

    void read_cmd(uint16_t cmd);

    Spi&     spi_;
    uint16_t cmd_[2];

    wibot::WaitHandler wh_;
    uint16_t           angle_;
    AS5047State        state_;
    ParityValidator    parity_;
};

}  // namespace wibot

#endif  // WWMOTOR_LIBS_WWDEVICE_DEVICE_AS5047SPI_HPP_
