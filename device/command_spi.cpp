#include "command_spi.hpp"
#define LOG_MODULE "cmdspi"
#include "log.h"

namespace wibot::device
{

using namespace wibot::accessor;
using namespace wibot::peripheral;

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
        if (frame.commandMode != CommandFrameMode::Skip)
        {
            rst = _do_step_send(CommandFramePhase::Command, &frame.commandId, 1, DataWidth::Bit8,
                                true, scope);
            if (rst != Result::OK)
            {
                break;
            }
        }

        if (frame.addressMode != CommandFrameMode::Skip)
        {
            rst = _do_step_send(CommandFramePhase::Address, &frame.address, 1, frame.addressBits,
                                true, scope);
            if (rst != Result::OK)
            {
                break;
            }
        }

        if (frame.altDataMode != CommandFrameMode::Skip)
        {
            rst = _do_step_send(CommandFramePhase::AltData, &frame.altData, 1, frame.altDataBits,
                                true, scope);
            if (rst != Result::OK)
            {
                break;
            }
        }

        if (frame.dummyCycles != 0)
        {
            rst = _do_step_send(CommandFramePhase::DummyCycle, nullptr, frame.dummyCycles,
                                DataWidth::Bit8, true, scope);
            if (rst != Result::OK)
            {
                break;
            }
        }

        if (frame.dataSize > 0 && frame.dataMode != CommandFrameMode::Skip)
        {
            rst = _do_step_send(CommandFramePhase::AltData, frame.data, frame.dataSize,
                                frame.dataBits, frame.isWrite, scope);
            if (rst != Result::OK)
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
    case CommandFramePhase::Command:
    case CommandFramePhase::Address:
    case CommandFramePhase::AltData:
        _spi.config_get().dataWidth = dataWidth;
        rst = _spi.write(false, data, size, *_waitHandler);
        break;
    case CommandFramePhase::DummyCycle:
        _waitHandler->set_value((void *)Result::NotSupport);
        _waitHandler->error_set(this);
        break;
    case CommandFramePhase::Data:
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
        rst = Result::NotSupport;
        break;
    }
    if (rst != Result::OK)
    {
        return rst;
    }
    rst = _waitHandler->wait(scope, _timeout);
    if (rst != Result::OK)
    {
        return rst;
    }
    return rst;
};

Result CommandSpi::media_session_start()
{
    _spi.session_begin();
    _waitHandler->done_set(this);
    return Result::OK;
};
Result CommandSpi::media_session_finish()
{
    _spi.session_end();
    _waitHandler->done_set(this);
    return Result::OK;
};

} // namespace wibot::device
