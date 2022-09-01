#include "command_spi.hpp"
#define LOG_MODULE "cmdspi"
#include "log.h"

namespace ww::device
{

using namespace ww::accessor;
using namespace ww::peripheral;

CommandSpi::CommandSpi(SpiWithPins &spi, uint32_t timeout) : Command(timeout), _spi(spi)
{
    BASE_INIT_ERROR_CHECK()
    MEMBER_INIT_ERROR_CHECK(spi)

    _spi.pinconfig_get().autoCs = false;
};

Result CommandSpi::media_operate(CommandFramePhase phase, void *data, uint32_t dataSize,
                                 DataWidth dataWidth, bool isWrite, WaitHandler &waitHandler)
{
    Result rst;
    switch (phase)
    {
    case CommandFramePhase_Command:
    case CommandFramePhase_Address:
    case CommandFramePhase_AltData:
        _spi.config_get().dataWidth = dataWidth;
        rst = _spi.write(false, data, dataSize, waitHandler);
        break;
    case CommandFramePhase_DummyCycle:
        waitHandler.set_value((void *)Result_NotSupport);
        waitHandler.error_set(this);
        break;
    case CommandFramePhase_Data:
        _spi.config_get().dataWidth = dataWidth;
        if (isWrite)
        {
            rst = _spi.write(true, data, dataSize, waitHandler);
        }
        else
        {
            rst = _spi.read(true, data, dataSize, waitHandler);
        }
        break;
    default:
        rst = Result_NotSupport;
        break;
    }
    return rst;
};
Result CommandSpi::media_session_start(WaitHandler &waitHandler)
{
    _spi.session_begin();
    waitHandler.done_set(this);
    return Result_OK;
};
Result CommandSpi::media_session_finish(WaitHandler &waitHandler)
{
    _spi.session_end();
    waitHandler.done_set(this);
    return Result_OK;
};

} // namespace ww::device