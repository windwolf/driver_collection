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
    Result _init() override;
    void _deinit() override;

  protected:
    Result media_session_start(WaitHandler &waitHandler) override;
    Result media_session_finish(WaitHandler &waitHandler) override;
    Result media_send(CommandFrame &frame, WaitHandler &waitHandler) override;

  private:
    QSPI &_qspi;
};
} // namespace ww::device

#endif // __WWDEVICE_COMMAND_SPI_HPP__
