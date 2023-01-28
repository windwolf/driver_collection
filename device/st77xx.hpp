#ifndef __WWDEVICE_ST77XX_HPP__
#define __WWDEVICE_ST77XX_HPP__

#include "command.hpp"
#include "command_spi.hpp"
#include "os.hpp"
#include "soc.hpp"

namespace wibot::device
{
using namespace wibot::accessor;
using namespace wibot::peripheral;

struct ST77xxConfig
{
    uint16_t width;
    uint16_t height;
    uint8_t colorMode;
    uint8_t orientation;
    uint16_t xOffset;
    uint16_t yOffset;
};

class ST77xx : public Initializable, public Configurable<ST77xxConfig>
{
  public:
    ST77xx(CommandSpi &cmdSpi, EventGroup& eventGroup);
    Result _init() override;
    void _deinit() override;

  protected:
    uint8_t _cmdData[16];
    uint8_t _pvGamma[16];
    uint8_t _nvGamma[16];

    static void _setup_command(CommandFrame &cmd);
    Result _command(uint8_t cmdId);
    Result _command_data(uint8_t cmdId, void *data, uint16_t size, DataWidth dataWidth,
                         bool isWrite);

  private:
    CommandSpi &_cmdSpi;
    WaitHandler _waitHandler;
};

} // namespace wibot::device

#endif // __WWDEVICE_ST77XX_HPP__
