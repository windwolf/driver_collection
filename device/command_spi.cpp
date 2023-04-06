#include "command_spi.hpp"

#include "log.h"
LOGGER("cmdspi")

namespace wibot::device {

using namespace wibot::accessor;
using namespace wibot::peripheral;

CommandSpi::CommandSpi(SpiWithPins& spi, uint32_t timeout) : Command(timeout), _spi(spi){};

Result CommandSpi::_init() {
    INIT_BEGIN()
    MEMBER_INIT_ERROR_CHECK(_spi)

    _spi.pinconfig_get().autoCs = false;
    INIT_END();
};
void CommandSpi::_deinit(){MEMBER_DEINIT(_spi)};

Result CommandSpi::media_command_send(CommandFrame& frame, WaitHandler& waitHandler) {
    Result rst;
    do {
        auto wh = waitHandler.folk();
        if (frame.commandMode != CommandFrameMode::Skip) {
            rst = _do_step_send(CommandFramePhase::Command, &frame.commandId, 1, DataWidth::Bit8,
                                true, wh);
            if (rst != Result::OK) {
                break;
            }
            rst = wh.wait(_timeout);
            if (rst != Result::OK) {
                break;
            }
        }

        if (frame.addressMode != CommandFrameMode::Skip) {
            rst = _do_step_send(CommandFramePhase::Address, &frame.address, 1, frame.addressBits,
                                true, wh);
            if (rst != Result::OK) {
                break;
            }
            rst = wh.wait(_timeout);
            if (rst != Result::OK) {
                break;
            }
        }

        if (frame.altDataMode != CommandFrameMode::Skip) {
            rst = _do_step_send(CommandFramePhase::AltData, &frame.altData, 1, frame.altDataBits,
                                true, wh);
            if (rst != Result::OK) {
                break;
            }
            rst = wh.wait(_timeout);
            if (rst != Result::OK) {
                break;
            }
        }

        if (frame.dummyCycles != 0) {
            rst = _do_step_send(CommandFramePhase::DummyCycle, nullptr, frame.dummyCycles,
                                DataWidth::Bit8, true, wh);
            if (rst != Result::OK) {
                break;
            }
            rst = wh.wait(_timeout);
            if (rst != Result::OK) {
                break;
            }
        }

        if (frame.dataSize > 0 && frame.dataMode != CommandFrameMode::Skip) {
            rst = _do_step_send(CommandFramePhase::AltData, frame.data, frame.dataSize,
                                frame.dataBits, frame.isWrite, wh);
            if (rst != Result::OK) {
                break;
            }
            rst = wh.wait(_timeout);
            if (rst != Result::OK) {
                break;
            }
        }
    } while (0);

    waitHandler.done_set(this);
    return rst;
}

Result CommandSpi::_do_step_send(CommandFramePhase phase, void* data, uint32_t size,
                                 DataWidth dataWidth, bool isWrite, WaitHandler& waitHandler) {
    Result rst;
    switch (phase) {
        case CommandFramePhase::Command:
        case CommandFramePhase::Address:
        case CommandFramePhase::AltData:
            _spi.config.dataWidth = dataWidth;
            _spi.apply_config();
            rst = _spi.write(false, data, size, waitHandler);
            break;
        case CommandFramePhase::DummyCycle:
            waitHandler.set_value((void*)Result::NotSupport);
            waitHandler.error_set(this);
            break;
        case CommandFramePhase::Data:
            _spi.config.dataWidth = dataWidth;
            _spi.apply_config();
            if (isWrite) {
                rst = _spi.write(true, data, size, waitHandler);
            } else {
                rst = _spi.read(true, data, size, waitHandler);
            }
            break;
        default:
            rst = Result::NotSupport;
            break;
    }
    return rst;
};

Result CommandSpi::media_session_start(WaitHandler& waitHandler) {
    _spi.session_begin(waitHandler);
    return Result::OK;
};
Result CommandSpi::media_session_finish(WaitHandler& waitHandler) {
    _spi.session_end(waitHandler);
    return Result::OK;
};

}  // namespace wibot::device
