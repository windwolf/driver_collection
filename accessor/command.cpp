#include "command.hpp"

namespace ww::accessor
{

Command::Command(uint32_t timeout) : _timeout(timeout){};

Result Command::_do_step_send(CommandFramePhase phase, void *data, uint32_t size,
                              DataWidth dataWidth, bool isWrite, WaitHandler &waitHandler,
                              uint32_t scope)
{
    auto rst = media_step_send(phase, data, size, dataWidth, isWrite, waitHandler);
    if (rst != Result_OK)
    {
        return rst;
    }
    rst = waitHandler.wait(scope, _timeout);
    if (rst != Result_OK)
    {
        return rst;
    }
    return rst;
};
Result Command::media_send(CommandFrame &frame, WaitHandler &waitHandler)
{
    Result rst;
    uint32_t scope = waitHandler.scope_begin();
    do
    {
        if (frame.commandMode != CommandFrameMode_Skip)
        {
            rst = _do_step_send(CommandFramePhase_Command, &frame.commandId, 1, DATAWIDTH_8, true,
                                waitHandler, scope);
            if (rst != Result_OK)
            {
                break;
            }
        }

        if (frame.addressMode != CommandFrameMode_Skip)
        {
            rst = _do_step_send(CommandFramePhase_Address, &frame.address, 1, frame.addressBits,
                                true, waitHandler, scope);
            if (rst != Result_OK)
            {
                break;
            }
        }

        if (frame.altDataMode != CommandFrameMode_Skip)
        {
            rst = _do_step_send(CommandFramePhase_AltData, &frame.altData, 1, frame.altDataBits,
                                true, waitHandler, scope);
            if (rst != Result_OK)
            {
                break;
            }
        }

        if (frame.dummyCycles != 0)
        {
            rst = _do_step_send(CommandFramePhase_DummyCycle, nullptr, frame.dummyCycles,
                                DATAWIDTH_8, true, waitHandler, scope);
            if (rst != Result_OK)
            {
                break;
            }
        }

        if (frame.dataSize > 0 && frame.dataMode != CommandFrameMode_Skip)
        {
            rst = _do_step_send(CommandFramePhase_AltData, frame.data, frame.dataSize,
                                frame.dataBits, frame.isWrite, waitHandler, scope);
            if (rst != Result_OK)
            {
                break;
            }
        }

    } while (0);
    waitHandler.scope_end();
    return rst;
}

Result Command::send(CommandFrame &frame, WaitHandler &waitHandler)
{
    Result rst;
    if (_waitHandler != nullptr)
    {
        return Result_Busy;
    }
    _waitHandler = &waitHandler;

    uint32_t scope = waitHandler.scope_begin();

    do
    {
        rst = media_session_start(waitHandler);
        if (rst != Result_OK)
        {
            break;
        }
        rst = waitHandler.wait(scope, _timeout);
        if (rst != Result_OK)
        {
            break;
        }
        rst = media_send(frame, waitHandler);
        if (rst != Result_OK)
        {
            break;
        }

    } while (0);

    // TODO: error handler.
    media_session_finish(waitHandler);
    waitHandler.wait(scope, _timeout);

    waitHandler.scope_end();
    waitHandler.done_set(this);
    _waitHandler = nullptr;
    return rst;
}

} // namespace ww::accessor
