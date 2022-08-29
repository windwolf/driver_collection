#include "st77xx.hpp"

namespace ww::device
{
using namespace ww::accessor;
using namespace ww::peripheral;

ST77xx::ST77xx(CommandSpi &cmdSpi)
    : _cmdSpi(cmdSpi){MEMBER_INIT_ERROR_CHECK(_cmdSpi)};

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
    CommandFrame cmd;
    _setup_command(cmd);
    // LOG("ST77XX-CMD0: S: %x", cmdId)
    cmd.commandId = cmdId;
    cmd.dataSize = 0;
    rst = _cmdSpi.send(cmd);
    return rst;
};

Result ST77xx::_command_data(uint8_t cmdId, void *data, uint16_t size,
                             DataWidth dataWidth, bool isWrite)
{
    Result rst;
    CommandFrame cmd;
    _setup_command(cmd);
    cmd.commandId = cmdId;
    cmd.data = data;
    cmd.dataSize = size;
    cmd.isWrite = isWrite;
    cmd.dataBits = dataWidth;

    rst = _cmdSpi.send(cmd);
    return rst;
};

} // namespace ww::device
