#ifndef __WWDEVICE_COMMAND_SPI_HPP__
#define __WWDEVICE_COMMAND_SPI_HPP__
#include "command.hpp"
#include "qspi.hpp"
namespace wibot::device
{
using namespace wibot::accessor;
using namespace wibot::peripheral;

class CommandQSPI : public Initializable, public Command
{
  public:
    CommandQSPI(QSPI &qspi, uint32_t timeout);
    Result _init() override;
    void _deinit() override;

  protected:
    Result media_session_start(WaitHandler &waitHandler) override;
    Result media_session_finish(WaitHandler &waitHandler) override;
    Result media_send(CommandFrame &frame, WaitHandler &waitHandler) override;

  private:
    QSPI &_qspi;
};
} // namespace wibot::device

#endif // __WWDEVICE_COMMAND_SPI_HPP__
