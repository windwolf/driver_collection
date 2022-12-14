#include "command.hpp"

namespace wibot::accessor
{

Command::Command(uint32_t timeout) : _timeout(timeout){};

Result Command::send(CommandFrame &frame, WaitHandler &waitHandler)
{
    Result rst;
    if (_waitHandler != nullptr)
    {
        return Result::Busy;
    }
    _waitHandler = &waitHandler;

    uint32_t scope = waitHandler.scope_begin();

    do
    {
        rst = media_session_start();
        if (rst != Result::OK)
        {
            break;
        }
        rst = waitHandler.wait(scope, _timeout);
        if (rst != Result::OK)
        {
            break;
        }
        rst = media_command_send(frame);
        if (rst != Result::OK)
        {
            break;
        }

    } while (0);

    // TODO: error handler.
    media_session_finish();
    waitHandler.wait(scope, _timeout);

    waitHandler.scope_end();
    waitHandler.done_set(this);
    _waitHandler = nullptr;
    return rst;
}

} // namespace wibot::accessor
