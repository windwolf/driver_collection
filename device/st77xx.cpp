#include "st77xx.hpp"

namespace wibot::device {
using namespace wibot::accessor;
using namespace wibot::peripheral;

ST77xx::ST77xx(CommandSpi &cmdSpi, EventGroup &eventGroup)
    : _cmdSpi(cmdSpi), _waitHandler(eventGroup){};
Result ST77xx::_init() {
    _cmdSpi.init();
    return Result::OK;
};
void ST77xx::_deinit() {
    _cmdSpi.deinit();
};

void ST77xx::_setup_command(CommandFrame &cmd) {
    cmd.commandMode = CommandFrameMode::Line1;
    cmd.addressMode = CommandFrameMode::Skip;
    cmd.altDataMode = CommandFrameMode::Skip;
    cmd.dataMode    = CommandFrameMode::Line1;
    cmd.dummyCycles = 0;
};

Result ST77xx::_command(uint8_t cmdId) {
    Result rst;
    do {
        CommandFrame cmd;
        _setup_command(cmd);
        // LOG("ST77XX-CMD0: S: %x", cmdId)
        cmd.commandId = cmdId;
        cmd.dataSize  = 0;

        rst = _cmdSpi.send(cmd, _waitHandler);
        if (rst != Result::OK) {
            break;
        }
        rst = _waitHandler.wait(TIMEOUT_FOREVER);
        if (rst != Result::OK) {
            break;
        }
        return rst;
    } while (0);

    return rst;
};

Result ST77xx::_command_data(uint8_t cmdId, void *data, uint16_t size, DataWidth dataWidth,
                             bool isWrite) {
    Result rst;
    do {
        CommandFrame cmd;
        _setup_command(cmd);
        cmd.commandId = cmdId;
        cmd.data      = data;
        cmd.dataSize  = size;
        cmd.isWrite   = isWrite;
        cmd.dataBits  = dataWidth;

        rst = _cmdSpi.send(cmd, _waitHandler);
        if (rst != Result::OK) {
            break;
        }
        rst = _waitHandler.wait(TIMEOUT_FOREVER);
        if (rst != Result::OK) {
            break;
        }
        return rst;
    } while (0);
    return rst;
};

}  // namespace wibot::device
