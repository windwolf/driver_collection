#include "command_qspi.hpp"
#define LOG_MODULE "cmdqspi"
#include "log.h"

namespace ww::device
{

using namespace ww::accessor;
using namespace ww::peripheral;

CommandQSPI::CommandQSPI(QSPI &qspi, uint32_t timeout)
    : Command(timeout), _qspi(qspi){BASE_INIT_ERROR_CHECK() MEMBER_INIT_ERROR_CHECK(_qspi)};

Result CommandQSPI::media_step_send(CommandFramePhase phase, void *data, uint32_t dataSize,
                                    DataWidth dataWidth, bool isWrite, WaitHandler &waitHandler)
{
    return Result_OK;
};
Result CommandQSPI::media_session_start(WaitHandler &waitHandler)
{
    _spi.session_begin();
    waitHandler.done_set(this);
    return Result_OK;
};
Result CommandQSPI::media_session_finish(WaitHandler &waitHandler)
{
    _spi.session_end();
    waitHandler.done_set(this);
    return Result_OK;
};

Result CommandQSPI::media_send(CommandFrame &frame, WaitHandler &waitHandler);

} // namespace ww::device