#include "st77xx.hpp"

namespace ww::device
{
using namespace ww::accessor;
using namespace ww::peripheral;

ST77xx::ST77xx(CommandSpi &cmdSpi, WaitHandler &waitHandler)
    : _cmdSpi(cmdSpi), _waitHandler(waitHandler)
{
    MEMBER_INIT_ERROR_CHECK(_cmdSpi)
    _scope = _waitHandler.scope_begin();
};

ST77xxConfig &ST77xx::config_get()
{
    return _config;
};

void ST77xx::_setup_command(CommandFrame &cmd)
{
    cmd.commandMode = CommandFrameMode_1line;
    cmd.addressMode = CommandFrameMode_Skip;
    cmd.altDataMode = CommandFrameMode_Skip;
    cmd.dataMode = CommandFrameMode_1line;
    cmd.dummyCycles = 0;
};

Result ST77xx::_command(uint8_t cmdId)
{
    Result rst;
    do
    {
        CommandFrame cmd;
        _setup_command(cmd);
        // LOG("ST77XX-CMD0: S: %x", cmdId)
        cmd.commandId = cmdId;
        cmd.dataSize = 0;

        rst = _cmdSpi.send(cmd, _waitHandler);
        if (rst != Result_OK)
        {
            break;
        }
        rst = _waitHandler.wait(_scope, TIMEOUT_FOREVER);
        if (rst != Result_OK)
        {
            break;
        }
        return rst;
    } while (0);

    return rst;
};

Result ST77xx::_command_data(uint8_t cmdId, void *data, uint16_t size, DataWidth dataWidth,
                             bool isWrite)
{
    Result rst;
    do
    {
        CommandFrame cmd;
        _setup_command(cmd);
        cmd.commandId = cmdId;
        cmd.data = data;
        cmd.dataSize = size;
        cmd.isWrite = isWrite;
        cmd.dataBits = dataWidth;

        rst = _cmdSpi.send(cmd, _waitHandler);
        if (rst != Result_OK)
        {
            break;
        }
        rst = _waitHandler.wait(_scope, TIMEOUT_FOREVER);
        if (rst != Result_OK)
        {
            break;
        }
        return rst;
    } while (0);
    return rst;
};

} // namespace ww::device
