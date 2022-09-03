#ifndef __WWDEVICE_COMMAND_SPI_HPP__
#define __WWDEVICE_COMMAND_SPI_HPP__
#include "command.hpp"
#include "spi.hpp"
namespace ww::device
{
using namespace ww::accessor;
using namespace ww::peripheral;

class CommandSpi : public Initializable, public Command
{
  public:
    CommandSpi(SpiWithPins &spi, uint32_t timeout);
    virtual Result _init();
    virtual void _deinit();

  protected:
    virtual Result media_command_send(CommandFrame &frame);
    virtual Result media_session_start();
    virtual Result media_session_finish();

  private:
    SpiWithPins &_spi;
    Result _do_step_send(CommandFramePhase phase, void *data, uint32_t size,
                                     DataWidth dataWidth, bool isWrite,
                                     uint32_t scope);
};
} // namespace ww::device

#endif // __WWDEVICE_COMMAND_SPI_HPP__
