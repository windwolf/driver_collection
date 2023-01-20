#include "command.hpp"

namespace wibot::accessor
{

Command::Command(uint32_t timeout) : _timeout(timeout){};

Result Command::send(CommandFrame &frame, WaitHandler &waitHandler)
{
    Result rst;

    auto wh = waitHandler.folk();
    do
    {

        rst = media_session_start(wh);
        if (rst != Result::OK)
        {
            break;
        }
        rst = wh.wait(_timeout);
        if (rst != Result::OK)
        {
            break;
        }
        rst = media_command_send(frame, wh);
        if (rst != Result::OK)
        {
            break;
        }
        rst = wh.wait(_timeout);
        if (rst != Result::OK)
        {
            break;
        }

    } while (0);

    // TODO: error handler.
    media_session_finish(wh);
    wh.wait(_timeout);

    waitHandler.done_set(this);
    return rst;
}

} // namespace wibot::accessor
