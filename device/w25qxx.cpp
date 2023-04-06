#include "w25qxx.hpp"

/* Write Operations */
#define W25QXX_SPI_WRITE_ENABLE_CMD             0x06
#define W25QXX_SPI_VOLATILE_SR_WRITE_ENABLE_CMD 0x50
#define W25QXX_SPI_WRITE_DISABLE_CMD            0x04

/* Identification Operations */
#define W25QXX_SPI_RELEASE_POWER_DOWN_CMD 0xAB
#define W25QXX_SPI_READ_ID_CMD            0x90
#define W25QXX_SPI_READ_JEDEC_ID_CMD      0x9F
#define W25QXX_SPI_READ_UNIQUE_ID_CMD     0x4B

/* Read Operations */
#define W25QXX_SPI_READ_CMD      0x03
#define W25QXX_SPI_FAST_READ_CMD 0x0B

/* Program Operations */
#define W25QXX_SPI_PAGE_PROG_CMD 0x02

/* Erase Operations */
#define W25QXX_SPI_SECTOR_ERASE_4K_CMD 0x20
#define W25QXX_SPI_BLOCK_ERASE_32K_CMD 0x52
#define W25QXX_SPI_BLOCK_ERASE_64K_CMD 0xD8
#define W25QXX_SPI_CHIP_ERASE_CMD      0xC7

/* Regiser Operations */
#define W25QXX_SPI_READ_STATUS_REG1_CMD 0x05
#define W25QXX_SPI_READ_STATUS_REG2_CMD 0x35
#define W25QXX_SPI_READ_STATUS_REG3_CMD 0x15

#define W25QXX_SPI_WRITE_STATUS_REG1_CMD 0x01
#define W25QXX_SPI_WRITE_STATUS_REG2_CMD 0x31
#define W25QXX_SPI_WRITE_STATUS_REG3_CMD 0x11

/* Security Register Operations */
#define W25QXX_SPI_READ_SFDP_REG_CMD        0x5A
#define W25QXX_SPI_ERASE_SECURITY_REG_CMD   0x44
#define W25QXX_SPI_PROGRAM_SECURITY_REG_CMD 0x42
#define W25QXX_SPI_READ_SECURITY_REG_CMD    0x48

/* Lock Operations */
#define W25QXX_SPI_GLOBAL_BLOCK_LOCK_CMD       0x7E
#define W25QXX_SPI_GLOBAL_BLOCK_UNLOCK_CMD     0x98
#define W25QXX_SPI_READ_BLOCK_LOCK_CMD         0x3D
#define W25QXX_SPI_INDEVIDUAL_BLOCK_LOCK_CMD   0x36
#define W25QXX_SPI_INDEVIDUAL_BLOCK_UNLOCK_CMD 0x39

#define W25QXX_SPI_PROG_ERASE_SUSPEND_CMD 0x75
#define W25QXX_SPI_PROG_ERASE_RESUME_CMD  0x7A
#define W25QXX_SPI_PONER_DOWN_CMD         0xB9

#define W25QXX_SPI_ENTER_QPI_MODE_CMD 0x38
#define W25QXX_SPI_ENABLE_RESET_CMD   0x66
#define W25QXX_SPI_RESET_DEVICE_CMD   0x99

#define W25QXX_QSPI_INPUT_PAGE_PROG_CMD     0x32  // 1-1-4
#define W25QXX_QSPI_FAST_READ_OUTPUT_CMD    0x6B  // 1-1-4
#define W25QXX_QSPI_MFTR_DEVICE_ID_IO_CMD   0x94  // 1-4-4
#define W25QXX_QSPI_FAST_READ_IO_CMD        0xEB  // 1-4-4
#define W25QXX_QSPI_SET_BURST_WITH_WRAP_CMD 0x77  // 1-4-4

/* Write Operations */
#define W25QXX_QPI_WRITE_ENABLE_CMD             0x06
#define W25QXX_QPI_VOLATILE_SR_WRITE_ENABLE_CMD 0x50
#define W25QXX_QPI_WRITE_DISABLE_CMD            0x04

/* Identification Operations */
#define W25QXX_QPI_RELEASE_POWER_DOWN_CMD  0xAB
#define W25QXX_QPI_READ_ID_CMD             0x90
#define W25QXX_QPI_READ_JEDEC_ID_CMD       0x9F
#define W25QXX_QPI_SET_READ_PARAMETERS_CMD 0xC0

/* Read Operations */
#define W25QXX_QPI_FAST_READ_CMD            0x0B
#define W25QXX_QPI_BURST_READ_WITH_WRAP_CMD 0x0C
#define W25QXX_QPI_FAST_READ_QUAD_IO_CMD    0xEB

/* Program Operations */
#define W25QXX_QPI_PAGE_PROG_CMD 0x02

/* Erase Operations */
#define W25QXX_QPI_SECTOR_ERASE_4K_CMD 0x20
#define W25QXX_QPI_BLOCK_ERASE_32K_CMD 0x52
#define W25QXX_QPI_BLOCK_ERASE_64K_CMD 0xD8
#define W25QXX_QPI_CHIP_ERASE_CMD      0xC7

/* Regiser Operations */
#define W25QXX_QPI_READ_STATUS_REG1_CMD 0x05
#define W25QXX_QPI_READ_STATUS_REG2_CMD 0x35
#define W25QXX_QPI_READ_STATUS_REG3_CMD 0x15

#define W25QXX_QPI_WRITE_STATUS_REG1_CMD 0x01
#define W25QXX_QPI_WRITE_STATUS_REG2_CMD 0x31
#define W25QXX_QPI_WRITE_STATUS_REG3_CMD 0x11

/* Lock Operations */
#define W25QXX_QPI_GLOBAL_BLOCK_LOCK_CMD       0x7E
#define W25QXX_QPI_GLOBAL_BLOCK_UNLOCK_CMD     0x7E
#define W25QXX_QPI_READ_BLOCK_LOCK_CMD         0x3D
#define W25QXX_QPI_INDEVIDUAL_BLOCK_LOCK_CMD   0x36
#define W25QXX_QPI_INDEVIDUAL_BLOCK_UNLOCK_CMD 0x39

#define W25QXX_QPI_PROG_ERASE_SUSPEND_CMD 0x75
#define W25QXX_QPI_PROG_ERASE_RESUME_CMD  0x7A
#define W25QXX_QPI_PONER_DOWN_CMD         0xB9

#define W25QXX_QPI_ENTER_QPI_MODE_CMD 0x38
#define W25QXX_QPI_ENABLE_RESET_CMD   0x66
#define W25QXX_QPI_RESET_DEVICE_CMD   0x99
#define W25QXX_QPI_EXIT_QPI_MODE_CMD  0xFF

#define W25QXX_EVENT_OP_BUSY 0x01
#define W25QXX_EVENT_OP_CPLT 0x02

// #define W25QXX_EVENTS_OP_BUSY_SET(instance)
// EVENTS_SET_FLAGS(instance->events, W25QXX_EVENT_OP_BUSY) #define
// W25QXX_EVENTS_OP_BUSY_RESET(instance) EVENTS_RESET_FLAGS(instance->events,
// W25QXX_EVENT_OP_BUSY) #define W25QXX_EVENTS_DEVICE_BUSY_SET(instance)
// EVENTS_SET_FLAGS(instance->events, W25QXX_EVENT_DEVICE_BUSY) #define
// W25QXX_EVENTS_DEVICE_BUSY_RESET(instance)
// EVENTS_RESET_FLAGS(instance->events, W25QXX_EVENT_DEVICE_BUSY)

namespace wibot::device {
using namespace wibot::os;

W25QXX::W25QXX(SpiWithPins &spi, EventGroup &eg, uint32_t timeout)
    : _cmdSpi(spi, timeout), _timeout(timeout), waitHandler_(eg){};

Result W25QXX::_init() {
    _cmdSpi.init();
    return Result::OK;
};
void W25QXX::_deinit() {
    _cmdSpi.deinit();
};

Result W25QXX::_spi_cmd_send(CommandFrame &frame) {
    auto rst = _cmdSpi.send(frame, waitHandler_);
    if (rst != Result::OK) {
        return rst;
    }
    return waitHandler_.wait(_timeout);
};

Result W25QXX::reset() {
    Result rst;

    rst = _reset_cmd();
    if (rst != Result::OK) {
        return rst;
    }
    rst = _qpi_exit_cmd();
    if (rst != Result::OK) {
        return rst;
    }
    W25QXX_Status2Register reg2;
    rst = _status_get(2, reg2.value);
    if (rst != Result::OK) {
        return rst;
    }
    reg2.QE = 0;
    rst     = _status_set(2, reg2.value);
    if (rst != Result::OK) {
        return rst;
    }
    rst = _busy_wait();
    if (rst != Result::OK) {
        return rst;
    }

    _cmdMode = W25QXX_CommandMode_SPI;
    return rst;
};

Result W25QXX::mode_switch(W25QXX_CommandMode cmdMode) {
    Result rst;
    if (cmdMode == W25QXX_CommandMode_SPI) {
        if (_cmdMode == W25QXX_CommandMode_QPI) {
            rst = _qpi_exit_cmd();
            if (rst != Result::OK) {
                return rst;
            }
            _cmdMode = cmdMode;
        }
    } else {
        W25QXX_Status2Register reg2;
        if (_cmdMode == W25QXX_CommandMode_SPI) {
            if (_dummyCycles != 2 && _dummyCycles != 4 && _dummyCycles != 6 && _dummyCycles != 8) {
                _dummyCycles = 2;
            }
            rst = _status_get(2, reg2.value);
            if (rst != Result::OK) {
                return rst;
            }
            reg2.QE = 1;
            rst     = _status_set(2, reg2.value);
            if (rst != Result::OK) {
                return rst;
            }
            rst = _qpi_enter_cmd();
            if (rst != Result::OK) {
                return rst;
            }
            _cmdMode = cmdMode;
            rst      = _read_parameter_set();
            if (rst != Result::OK) {
                return rst;
            }
        }
    }
    return Result::OK;
};

Result W25QXX::block_erase(uint32_t address) {
    Result rst;
    rst = _write_enable_cmd();
    if (rst != Result::OK) {
        return rst;
    }

    rst = _erase_cmd(W25QXX_EraseMode_4K, address);
    if (rst != Result::OK) {
        return rst;
    }
    return _busy_wait();
};
Result W25QXX::chip_erase() {
    Result rst;
    rst = _write_enable_cmd();
    if (rst != Result::OK) {
        return rst;
    }
    rst = _chip_erase_cmd();
    if (rst != Result::OK) {
        return rst;
    }
    return _busy_wait();
};

Result W25QXX::media_read(void *data, uint32_t num, uint32_t size) {
    return _read_cmd((uint8_t *)data, num, size);
};
Result W25QXX::media_write(void *data, uint32_t num, uint32_t size) {
    Result rst;
    do {
        rst = _write_enable_cmd();
        if (rst != Result::OK) {
            break;
        }

        rst = _write_cmd((uint8_t *)data, num, size);
        if (rst != Result::OK) {
            break;
        }
        rst = _busy_wait();
    } while (0);
    return rst;
};

Result W25QXX::media_erase(uint32_t num, uint32_t size) {
    Result rst;
    do {
        uint32_t blkBeginAddr = num & ~(W25QXX_BLOCK_SIZE - 1);
        uint32_t blkEndAddr   = (num + size - 1) & ~(W25QXX_BLOCK_SIZE - 1);
        uint32_t curAddr      = blkBeginAddr;
        do {
            rst = block_erase(curAddr);
            if (rst != Result::OK) {
                break;
            }
            curAddr += W25QXX_BLOCK_SIZE;

        } while (curAddr <= blkEndAddr);
    } while (0);
    return rst;
};

void W25QXX::_cmdline_config(CommandFrame &frame, W25QXX_CMD_LINE_MODE lineMode) {
    frame.altDataMode = CommandFrameMode::Skip;
    switch (lineMode) {
        case W25QXX_CMD_LINE_MODE_111:
            frame.commandMode = CommandFrameMode::Line1;
            frame.addressMode = CommandFrameMode::Line1;

            frame.dataMode = CommandFrameMode::Line1;
            break;
        case W25QXX_CMD_LINE_MODE_110:
            frame.commandMode = CommandFrameMode::Line1;
            frame.addressMode = CommandFrameMode::Line1;
            frame.dataMode    = CommandFrameMode::Skip;
            break;
        case W25QXX_CMD_LINE_MODE_101:
            frame.commandMode = CommandFrameMode::Line1;
            frame.addressMode = CommandFrameMode::Skip;
            frame.dataMode    = CommandFrameMode::Line1;
            break;
        case W25QXX_CMD_LINE_MODE_100:
            frame.commandMode = CommandFrameMode::Line1;
            frame.addressMode = CommandFrameMode::Skip;
            frame.dataMode    = CommandFrameMode::Skip;
            break;
        case W25QXX_CMD_LINE_MODE_444:
            frame.commandMode = CommandFrameMode::Line4;
            frame.addressMode = CommandFrameMode::Line4;
            frame.dataMode    = CommandFrameMode::Line4;
            break;
        case W25QXX_CMD_LINE_MODE_440:
            frame.commandMode = CommandFrameMode::Line4;
            frame.addressMode = CommandFrameMode::Line4;
            frame.dataMode    = CommandFrameMode::Skip;
            break;
        case W25QXX_CMD_LINE_MODE_404:
            frame.commandMode = CommandFrameMode::Line4;
            frame.addressMode = CommandFrameMode::Skip;
            frame.dataMode    = CommandFrameMode::Line4;
            break;
        case W25QXX_CMD_LINE_MODE_400:
            frame.commandMode = CommandFrameMode::Line4;
            frame.addressMode = CommandFrameMode::Skip;
            frame.dataMode    = CommandFrameMode::Skip;
            break;
        default:
            break;
    }
    frame.dummyCycles = 0;
}

Result W25QXX::_status_get(uint8_t reg_num, uint8_t &status) {
    Result       rst;
    CommandFrame cmd;
    if (_cmdMode == W25QXX_CommandMode_QPI) {
        // QPI
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_404);
    } else {
        // SPI
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_101);
    }

    cmd.commandId = (reg_num == 1) ? W25QXX_SPI_READ_STATUS_REG1_CMD
                                   : ((reg_num == 2) ? W25QXX_SPI_READ_STATUS_REG2_CMD
                                                     : W25QXX_SPI_READ_STATUS_REG3_CMD);
    cmd.dataSize  = 1;
    cmd.dataBits  = DataWidth::Bit8;
    cmd.isWrite   = 0;
    cmd.data      = &status;

    rst = _spi_cmd_send(cmd);
    return rst;
};
Result W25QXX::_status_set(uint8_t reg_num, uint8_t status) {
    Result       rst;
    CommandFrame cmd;
    if (_cmdMode == W25QXX_CommandMode_QPI) {
        // QPI
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_404);
    } else {
        // SPI
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_101);
    }

    cmd.commandId = (reg_num == 1) ? W25QXX_SPI_WRITE_STATUS_REG1_CMD
                                   : ((reg_num == 2) ? W25QXX_SPI_WRITE_STATUS_REG2_CMD
                                                     : W25QXX_SPI_WRITE_STATUS_REG3_CMD);
    cmd.dataSize  = 1;
    cmd.dataBits  = DataWidth::Bit8;
    cmd.isWrite   = 1;
    cmd.data      = &status;

    rst = _spi_cmd_send(cmd);
    return rst;
};

Result W25QXX::id_read(uint32_t &mdId, uint32_t &jedecId) {
    Result       rst;
    CommandFrame cmd;
    if (_cmdMode == W25QXX_CommandMode_SPI) {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_111);
    } else {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_444);
    }
    cmd.commandId   = W25QXX_SPI_READ_ID_CMD;
    cmd.address     = 0x0000;
    cmd.addressBits = DataWidth::Bit16;
    cmd.data        = &mdId;
    cmd.dataBits    = DataWidth::Bit8;
    cmd.dataSize    = 3;
    rst             = _spi_cmd_send(cmd);
    if (rst != Result::OK) {
        return rst;
    }

    if (_cmdMode == W25QXX_CommandMode_SPI) {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_101);
    } else {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_404);
    }
    cmd.commandId = W25QXX_SPI_READ_JEDEC_ID_CMD;
    cmd.data      = &jedecId;
    cmd.dataBits  = DataWidth::Bit8;
    cmd.dataSize  = 3;
    rst           = _spi_cmd_send(cmd);
    return rst;
};
Result W25QXX::_busy_wait() {
    Result                 rst;
    W25QXX_Status1Register status1;
    do {
        rst = _status_get(1, status1.value);

        if (rst != Result::OK) {
            return rst;
        }
        if (status1.BUSY) {
            return Result::OK;
        } else {
            Thread::sleep(1);
        }
    } while (1);
};

Result W25QXX::_read_parameter_set() {
    Result       rst;
    CommandFrame cmd;
    if (_cmdMode == W25QXX_CommandMode_SPI) {
        // QPI
        uint8_t params;
        switch (this->_dummyCycles) {
            case 2:
                params = 0x00;
                break;
            case 4:
                params = 0x04;
                break;
            case 6:
                params = 0x08;
                break;
            case 8:
                params = 0x0C;
                break;
            default:
                break;
        }

        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_404);
        cmd.commandId = W25QXX_QPI_SET_READ_PARAMETERS_CMD;
        cmd.dataSize  = 1;
        cmd.dataBits  = DataWidth::Bit8;
        cmd.isWrite   = 1;
        cmd.data      = &params;
        rst           = _spi_cmd_send(cmd);
        return rst;
    } else {
        // SPI
        return Result::NotSupport;
    }
};
Result W25QXX::_write_enable_cmd() {
    Result       rst;
    CommandFrame cmd;
    if (_cmdMode == W25QXX_CommandMode_SPI) {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_100);
    } else {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_400);
    }
    cmd.commandId = W25QXX_SPI_WRITE_ENABLE_CMD;

    rst = _spi_cmd_send(cmd);
    return rst;
};

Result W25QXX::_write_cmd(uint8_t *pData, uint32_t writeAddr, uint32_t size) {
    Result       rst;
    CommandFrame cmd;
    if (_cmdMode == W25QXX_CommandMode_SPI) {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_111);
    } else {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_444);
    }

    cmd.commandId   = W25QXX_SPI_PAGE_PROG_CMD;
    cmd.address     = writeAddr;
    cmd.addressBits = DataWidth::Bit24;
    cmd.isWrite     = 1;
    cmd.data        = pData;
    cmd.dataSize    = size;
    cmd.dataBits    = DataWidth::Bit8;

    rst = _spi_cmd_send(cmd);
    return rst;
};
Result W25QXX::_read_cmd(uint8_t *pData, uint32_t readAddr, uint32_t size) {
    Result       rst;
    CommandFrame cmd;
    if (_cmdMode == W25QXX_CommandMode_SPI) {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_111);
        cmd.commandId   = W25QXX_SPI_READ_CMD;
        cmd.dummyCycles = 0;
    } else {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_444);
        cmd.commandId   = W25QXX_QPI_FAST_READ_CMD;
        cmd.dummyCycles = _dummyCycles;
    }
    cmd.address     = readAddr;
    cmd.addressBits = DataWidth::Bit24;

    cmd.data     = pData;
    cmd.dataSize = size;
    cmd.dataBits = DataWidth::Bit8;
    cmd.isWrite  = 0;

    rst = _spi_cmd_send(cmd);
    return rst;
};
Result W25QXX::_qpi_enter_cmd() {
    Result       rst;
    CommandFrame cmd;
    _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_100);
    cmd.commandId = W25QXX_SPI_ENTER_QPI_MODE_CMD;
    rst           = _spi_cmd_send(cmd);
    return rst;
};
Result W25QXX::_qpi_exit_cmd() {
    Result       rst;
    CommandFrame cmd;
    _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_400);
    cmd.commandId = W25QXX_QPI_EXIT_QPI_MODE_CMD;
    rst           = _spi_cmd_send(cmd);
    return rst;
};
Result W25QXX::_reset_cmd() {
    Result       rst;
    CommandFrame cmd;
    if (_cmdMode == W25QXX_CommandMode_SPI) {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_100);
    } else {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_400);
    }

    cmd.commandId = W25QXX_QPI_ENABLE_RESET_CMD;
    rst           = _spi_cmd_send(cmd);
    if (rst != Result::OK) {
        return rst;
    }
    cmd.commandId = W25QXX_QPI_RESET_DEVICE_CMD;
    rst           = _spi_cmd_send(cmd);
    return rst;
};

Result W25QXX::_erase_cmd(W25QXX_EraseMode mode, uint32_t address) {
    Result       rst;
    CommandFrame cmd;
    if (_cmdMode == W25QXX_CommandMode_SPI) {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_110);
    } else {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_440);
    }
    cmd.commandId   = (mode == W25QXX_EraseMode_4K)
                          ? W25QXX_SPI_SECTOR_ERASE_4K_CMD
                          : ((mode == W25QXX_EraseMode_32K) ? W25QXX_SPI_BLOCK_ERASE_32K_CMD
                                                            : W25QXX_SPI_BLOCK_ERASE_64K_CMD);
    cmd.address     = address;
    cmd.addressBits = DataWidth::Bit24;

    rst = _spi_cmd_send(cmd);
    return rst;
};
Result W25QXX::_chip_erase_cmd() {
    Result       rst;
    CommandFrame cmd;
    if (_cmdMode == W25QXX_CommandMode_SPI) {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_100);
    } else {
        _cmdline_config(cmd, W25QXX_CMD_LINE_MODE_440);
    }
    cmd.commandId = W25QXX_SPI_CHIP_ERASE_CMD;

    rst = _spi_cmd_send(cmd);
    return rst;
};

BlockableW25QXX::BlockableW25QXX(W25QXX &w25qxx, Buffer8 buffer)
    : Block(buffer), _w25qxx(w25qxx){

                     };
Result BlockableW25QXX::_init() {
    _w25qxx.init();
    return config_set(BlockConfig{
        .readBlockSize        = 0,
        .writeBlockSize       = W25QXX_PAGE_SIZE,
        .eraseBlockSize       = W25QXX_BLOCK_SIZE,
        .readMode             = BlockMode::Random,
        .writeMode            = BlockMode::Wrap,
        .eraseMode            = BlockMode::RandomBlock,
        .needEraseBeforeWrite = true,
    });
};
void BlockableW25QXX::_deinit() {
    _w25qxx.deinit();
};

Result BlockableW25QXX::media_read(void *data, uint32_t num, uint32_t size) {
    return _w25qxx.media_read(data, num, size);
};

Result BlockableW25QXX::media_write(void *data, uint32_t num, uint32_t size) {
    return _w25qxx.media_write(data, num, size);
};

Result BlockableW25QXX::media_erase(uint32_t num, uint32_t size) {
    return _w25qxx.media_erase(num, size);
}

}  // namespace wibot::device
