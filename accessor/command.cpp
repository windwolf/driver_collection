#include "command.hpp"

namespace ww::accessor
{
Command::Command(EventGroup &eventGroup, uint32_t doneFlag, uint32_t errorFlag,
                 uint32_t readyFlag, uint32_t timeout)
    : _eventGroup(eventGroup), _waitHandler(eventGroup, doneFlag, errorFlag),
      _timeout(timeout), _readyFlag(readyFlag)
{
    initErrorCode = _eventGroup.set(_readyFlag);
};

Result Command::_session_begin()
{
    uint32_t events;
    auto rst = _eventGroup.wait(_readyFlag, events, EventOptions_WaitForAll,
                                TIMEOUT_FOREVER);
    if (rst != Result_OK)
    {
        _session_end();
        return rst;
    }
    rst = media_session_start(_waitHandler);
    if (rst != Result_OK)
    {
        _session_end();
        return rst;
    }
    rst = _waitHandler.wait(TIMEOUT_FOREVER);
    if (rst != Result_OK)
    {
        _session_end();
        return rst;
    }
    return rst;
};
Result Command::_session_end()
{
    // TODO: error handler.
    media_session_finish(_waitHandler);
    _waitHandler.wait(TIMEOUT_FOREVER);
    _eventGroup.set(_readyFlag);
    return Result_OK;
};

Result Command::_do_send(CommandFramePhase phase, void *data, uint32_t size,
                         DataWidth dataWidth, bool isWrite)
{
    auto rst =
        media_operate(phase, data, size, dataWidth, isWrite, _waitHandler);
    if (rst != Result_OK)
    {
        _session_end();
        return rst;
    }
    rst = _waitHandler.wait(TIMEOUT_FOREVER);
    if (rst != Result_OK)
    {
        _session_end();
        return rst;
    }
    return rst;
};

Result Command::send(CommandFrame &frame)
{

    auto rst = _session_begin();
    if (rst != Result_OK)
    {
        return rst;
    }

    if (frame.commandMode != CommandFrameMode_Skip)
    {
        rst = _do_send(CommandFramePhase_Command, &frame.commandId, 1,
                       DATAWIDTH_8, true);
        if (rst != Result_OK)
        {
            return rst;
        }
    }

    if (frame.addressMode != CommandFrameMode_Skip)
    {
        rst = _do_send(CommandFramePhase_Address, &frame.address, 1,
                       frame.addressBits, true);
        if (rst != Result_OK)
        {
            return rst;
        }
    }

    if (frame.altDataMode != CommandFrameMode_Skip)
    {
        rst = _do_send(CommandFramePhase_AltData, &frame.altData, 1,
                       frame.altDataBits, true);
        if (rst != Result_OK)
        {
            return rst;
        }
    }
    if (frame.dummyCycles != 0)
    {
        rst = _do_send(CommandFramePhase_DummyCycle, nullptr, frame.dummyCycles,
                       DATAWIDTH_8, true);
        if (rst != Result_OK)
        {
            return rst;
        }
    }

    if (frame.dataSize > 0 && frame.dataMode != CommandFrameMode_Skip)
    {
        rst = _do_send(CommandFramePhase_AltData, frame.data, frame.dataSize,
                       frame.dataBits, frame.isWrite);
        if (rst != Result_OK)
        {
            return rst;
        }
    }
    return _session_end();
};
} // namespace ww::accessor
