#include "command_spi.hpp"
#define LOG_MODULE "cmdspi"
#include "log.h"

namespace ww::device
{

using namespace ww::accessor;
using namespace ww::peripheral;

CommandSpi::CommandSpi(SpiWithPins &spi, uint32_t timeout) : Command(timeout), _spi(spi){};

Result CommandSpi::_init()
{
    INIT_BEGIN()
    MEMBER_INIT_ERROR_CHECK(_spi)

    _spi.pinconfig_get().autoCs = false;
    INIT_END();
};
void CommandSpi::_deinit(){MEMBER_DEINIT(_spi)};

Result CommandSpi::media_command_send(CommandFrame &frame)
{
    Result rst;
    uint32_t scope = _waitHandler->scope_begin();
    do
    {
        if (frame.commandMode != CommandFrameMode_Skip)
        {
            rst = _do_step_send(CommandFramePhase_Command, &frame.commandId, 1, DATAWIDTH_8, true,
                                scope);
            if (rst != Result_OK)
            {
                break;
            }
        }

        if (frame.addressMode != CommandFrameMode_Skip)
        {
            rst = _do_step_send(CommandFramePhase_Address, &frame.address, 1, frame.addressBits,
                                true, scope);
            if (rst != Result_OK)
            {
                break;
            }
        }

        if (frame.altDataMode != CommandFrameMode_Skip)
        {
            rst = _do_step_send(CommandFramePhase_AltData, &frame.altData, 1, frame.altDataBits,
                                true, scope);
            if (rst != Result_OK)
            {
                break;
            }
        }

        if (frame.dummyCycles != 0)
        {
            rst = _do_step_send(CommandFramePhase_DummyCycle, nullptr, frame.dummyCycles,
                                DATAWIDTH_8, true, scope);
            if (rst != Result_OK)
            {
                break;
            }
        }

        if (frame.dataSize > 0 && frame.dataMode != CommandFrameMode_Skip)
        {
            rst = _do_step_send(CommandFramePhase_AltData, frame.data, frame.dataSize,
                                frame.dataBits, frame.isWrite, scope);
            if (rst != Result_OK)
            {
                break;
            }
        }

    } while (0);
    _waitHandler->scope_end();
    return rst;
}

Result CommandSpi::_do_step_send(CommandFramePhase phase, void *data, uint32_t size,
                                 DataWidth dataWidth, bool isWrite, uint32_t scope)
{
    Result rst;
    switch (phase)
    {
    case CommandFramePhase_Command:
    case CommandFramePhase_Address:
    case CommandFramePhase_AltData:
        _spi.config_get().dataWidth = dataWidth;
        rst = _spi.write(false, data, size, *_waitHandler);
        break;
    case CommandFramePhase_DummyCycle:
        _waitHandler->set_value((void *)Result_NotSupport);
        _waitHandler->error_set(this);
        break;
    case CommandFramePhase_Data:
        _spi.config_get().dataWidth = dataWidth;
        if (isWrite)
        {
            rst = _spi.write(true, data, size, *_waitHandler);
        }
        else
        {
            rst = _spi.read(true, data, size, *_waitHandler);
        }
        break;
    default:
        rst = Result_NotSupport;
        break;
    }
    if (rst != Result_OK)
    {
        return rst;
    }
    rst = _waitHandler->wait(scope, _timeout);
    if (rst != Result_OK)
    {
        return rst;
    }
    return rst;
};

Result CommandSpi::media_session_start( )
{
    _spi.session_begin();
    _waitHandler->done_set(this);
    return Result_OK;
};
Result CommandSpi::media_session_finish( )
{
    _spi.session_end();
    _waitHandler->done_set(this);
    return Result_OK;
};

} // namespace ww::device