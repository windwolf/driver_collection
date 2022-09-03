#ifndef __WWDEVICE_ST77XX_HPP__
#define __WWDEVICE_ST77XX_HPP__

#include "command.hpp"
#include "command_spi.hpp"
#include "os.hpp"
#include "soc.hpp"

namespace ww::device
{
using namespace ww::accessor;
using namespace ww::peripheral;

struct ST77xxConfig
{
    uint16_t width;
    uint16_t height;
    uint8_t colorMode;
    uint8_t orientation;
    uint16_t xOffset;
    uint16_t yOffset;
};

class ST77xx : Initializable
{
  public:
    ST77xx(CommandSpi &cmdSpi, WaitHandler &waitHandler);
    virtual Result _init();
    virtual void _deinit();
    ST77xxConfig &config_get();

  protected:
    ST77xxConfig _config;
    uint8_t _cmdData[16];
    uint8_t _pvGamma[16];
    uint8_t _nvGamma[16];

    static void _setup_command(CommandFrame &cmd);
    Result _command(uint8_t cmdId);
    Result _command_data(uint8_t cmdId, void *data, uint16_t size, DataWidth dataWidth,
                         bool isWrite);

  private:
    CommandSpi &_cmdSpi;
    WaitHandler &_waitHandler;
    uint32_t _scope;
};

} // namespace ww::device

#endif // __WWDEVICE_ST77XX_HPP__