#ifndef __WWDEVICE_W25QXX_HPP__
#define __WWDEVICE_W25QXX_HPP__

#endif // __WWDEVICE_W25QXX_HPP__

#include "base.hpp"
#include "block.hpp"
#include "command.hpp"
#include "command_spi.hpp"
#include "peripheral.hpp"

namespace wibot::device
{
using namespace wibot::peripheral;
using namespace wibot::accessor;
#define W25QXX_PAGE_SIZE 256
#define W25QXX_BLOCK_SIZE 4096
enum W25QXX_CommandMode
{
    W25QXX_CommandMode_SPI,
    // W25QXX_CommandMode_QSPI,
    W25QXX_CommandMode_QPI,
};

enum W25QXX_EraseMode
{
    W25QXX_EraseMode_4K,
    W25QXX_EraseMode_32K,
    W25QXX_EraseMode_64K,
    W25QXX_EraseMode_CHIP,
};

enum W25QXX_CMD_LINE_MODE : uint8_t
{
    W25QXX_CMD_LINE_MODE_111 = 0x15,
    W25QXX_CMD_LINE_MODE_110 = 0x14,
    W25QXX_CMD_LINE_MODE_101 = 0x11,
    W25QXX_CMD_LINE_MODE_100 = 0x10,
    W25QXX_CMD_LINE_MODE_444 = 0x2A,
    W25QXX_CMD_LINE_MODE_440 = 0x28,
    W25QXX_CMD_LINE_MODE_404 = 0x22,
    W25QXX_CMD_LINE_MODE_400 = 0x20,
};

union W25QXX_Status1Register {
    uint8_t value;
    struct
    {
        uint8_t BUSY : 1;
        uint8_t WEL : 1;
        uint8_t BP : 3;
        uint8_t TB : 1;
        uint8_t SEC : 1;
        uint8_t SRP : 1;
    };
};
union W25QXX_Status2Register {
    uint8_t value;
    struct
    {
        uint8_t SRL : 1;
        uint8_t QE : 1;
        uint8_t : 1;
        uint8_t LB : 3;
        uint8_t CMP : 1;
        uint8_t SUS : 1;
    };
};
union W25QXX_Status3Register {
    uint8_t value;
    struct
    {
        uint8_t : 2;
        uint8_t WPS : 1;
        uint8_t : 2;
        uint8_t DRV : 2;
        uint8_t HOLD_RST : 1;
    };
};

class W25QXX : public Initializable
{
  public:
    W25QXX(SpiWithPins &spi, uint32_t timeout);
    Result _init() override;
    void _deinit() override;
    Result reset();

    Result mode_switch(W25QXX_CommandMode cmdMode);

    Result block_erase(uint32_t address);
    Result chip_erase();
    Result id_read(uint32_t &mdId, uint32_t &jedecId);

    Result media_read(void *data, uint32_t num, uint32_t size, WaitHandler &waitHandler);
    Result media_write(void *data, uint32_t num, uint32_t size, WaitHandler &waitHandler);
    Result media_erase(uint32_t num, uint32_t size, WaitHandler &waitHandler);

  private:
    Result _status_get(uint8_t reg_num, uint8_t &status);
    Result _status_set(uint8_t reg_num, uint8_t status);

    Result _busy_wait();
    Result _read_parameter_set();
    static void _cmdline_config(CommandFrame &frame, W25QXX_CMD_LINE_MODE lineMode);
    Result _write_enable_cmd();
    Result _write_cmd(uint8_t *pData, uint32_t writeAddr, uint32_t dataSize);
    Result _read_cmd(uint8_t *pData, uint32_t readAddr, uint32_t size);

    Result _erase_cmd(W25QXX_EraseMode mode, uint32_t address);
    Result _chip_erase_cmd();
    Result _qpi_enter_cmd();
    Result _qpi_exit_cmd();
    Result _reset_cmd();
    Result _spi_cmd_send(CommandFrame &frame);

  private:
    CommandSpi _cmdSpi;
    uint32_t _timeout;
    W25QXX_CommandMode _cmdMode;
    uint8_t _dummyCycles;
    WaitHandler *_waitHandler;
    uint32_t _scope;
};

class BlockableW25QXX : public Initializable, public Block
{
  public:
    BlockableW25QXX(W25QXX &w25qxx, Buffer8 buffer);
    Result _init() override;
    void _deinit() override;

  protected:
    W25QXX &_w25qxx;
    Result media_read(void *data, uint32_t num, uint32_t size,
                      WaitHandler &waitHandler) override;
    Result media_write(void *data, uint32_t num, uint32_t size,
                       WaitHandler &waitHandler) override;
    Result media_erase(uint32_t num, uint32_t size, WaitHandler &waitHandler) override;
};

} // namespace wibot::device
