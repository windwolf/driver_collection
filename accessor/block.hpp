#ifndef __WWDEVICE_ACCESSOR_BLOCK_HPP__
#define __WWDEVICE_ACCESSOR_BLOCK_HPP__

#include "base.hpp"
#include "buffer.hpp"

#include "wait_handler.hpp"

namespace wibot::accessor
{
using namespace wibot::os;

enum class BlockMode : uint8_t
{
    Random = 0x00,
    Wrap = 0x01,
    Blockwise = 0x02,
    RandomBlock = 0x03,
    Block = 0x04,
};

struct BlockConfig
{

    uint32_t readBlockSize;
    uint32_t writeBlockSize;
    uint32_t eraseBlockSize;
    BlockMode readMode;
    BlockMode writeMode;
    BlockMode eraseMode;
    bool needEraseBeforeWrite;
};

class Block
{
  public:
    Block(Buffer8 &buffer);
    Result config_set(const BlockConfig &config);

    Result read(void *data, uint32_t address, uint32_t size);
    Result write(void *data, uint32_t address, uint32_t size);
    Result erase(uint32_t address, uint32_t size);

  protected:
    Result _read(void *data, uint32_t address, uint32_t size);

    virtual Result media_read(void *data, uint32_t num, uint32_t size) = 0;
    virtual Result media_write(void *data, uint32_t num, uint32_t size) = 0;
    virtual Result media_erase(uint32_t num, uint32_t size) = 0;

  private:
    struct
    {
        uint32_t _maxBlockSize;
        uint32_t _readBlockSizeBits;
        uint32_t _writeBlockSizeBits;
        uint32_t _eraseBlockSizeBits;
        uint32_t _readBlockSizeMask;
        uint32_t _writeBlockSizeMask;
        uint32_t _eraseBlockSizeMask;
    } _procedConfig;
    BlockConfig _config;
    Buffer8 _buffer;
    Result _write_directly(void *data, uint32_t address, uint32_t size);
    Result _process_config();
};
} // namespace wibot::accessor

#endif // __WWDEVICE_ACCESSOR_BLOCK_HPP__
