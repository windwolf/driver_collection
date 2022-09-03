#ifndef __WWDEVICE_COMMAND_SPI_HPP__
#define __WWDEVICE_COMMAND_SPI_HPP__
#include "command.hpp"
#include "qspi.hpp"
namespace ww::device
{
using namespace ww::accessor;
using namespace ww::peripheral;

class CommandQSPI : public Initializable, public Command
{
  public:
    CommandQSPI(QSPI &qspi, uint32_t timeout);
    virtual Result _init();
    virtual void _deinit();
  protected:
    virtual Result media_operate(CommandFramePhase phase, void *data, uint32_t dataSize,
                                 DataWidth dataWidth, bool isWrite, WaitHandler &waitHandler);
    virtual Result media_session_start(WaitHandler &waitHandler);
    virtual Result media_session_finish(WaitHandler &waitHandler);
    virtual Result media_send(CommandFrame &frame, WaitHandler &waitHandler);

  private:
    QSPI &_qspi;
};
} // namespace ww::device

#endif // __WWDEVICE_COMMAND_SPI_HPP__
