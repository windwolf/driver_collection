#include "block.hpp"

#include "string.h"
#include "stdlib.h"
#include "math.h"

#define LOG_MODULE "block"
#include "log.h"

#define min(a, b) (((a) <= (b)) ? (a) : (b))
#define max(a, b) (((a) >= (b)) ? (a) : (b))

namespace wibot::accessor
{

Block::Block(Buffer8 &buffer) : _buffer(buffer){};

Result Block::config_set(const BlockConfig &config)
{
    _config = config;
    return _process_config();
};
Result Block::_process_config()
{
    if (_config.readMode == BlockMode::RandomBlock)
    {
        LOG_E("read should not RANDOM_BLOCK");
        return Result::InvalidParameter;
    }
    if (_config.writeMode == BlockMode::RandomBlock)
    {
        LOG_E("write should not RANDOM_BLOCK");
        return Result::InvalidParameter;
    }
    if (_config.writeBlockSize > _config.eraseBlockSize)
    {
        LOG_E("write block should not be great then erase block size.");
        return Result::InvalidParameter;
    }
    if (_buffer.size < max(_config.readBlockSize, _config.eraseBlockSize))
    {
        return Result::InvalidParameter;
    }

    uint32_t maxBlockSize = (_config.readBlockSize > _config.writeBlockSize)
                                ? _config.readBlockSize
                                : _config.writeBlockSize;
    maxBlockSize = (maxBlockSize > _config.eraseBlockSize) ? maxBlockSize : _config.eraseBlockSize;
    _procedConfig._maxBlockSize = maxBlockSize;

    _procedConfig._readBlockSizeBits = fast_log2(_config.readBlockSize);
    _procedConfig._writeBlockSizeBits = fast_log2(_config.writeBlockSize);
    _procedConfig._eraseBlockSizeBits = fast_log2(_config.eraseBlockSize);
    _procedConfig._readBlockSizeMask = _config.readBlockSize - 1;
    _procedConfig._writeBlockSizeMask = _config.writeBlockSize - 1;
    _procedConfig._eraseBlockSizeMask = _config.eraseBlockSize - 1;
    return Result::OK;
};

Result Block::read(void *data, uint32_t address, uint32_t size)
{
    Result rst;

    do
    {
        if (_config.readMode == BlockMode::Random)
        {
            rst = media_read(data, address, size);
            if (rst != Result::OK)
            {
                break;
            }
        }
        else if (_config.readMode == BlockMode::Blockwise)
        {
            uint32_t blkAddress = address & ~(_procedConfig._readBlockSizeMask);
            uint32_t blkSize = size & ~(_procedConfig._readBlockSizeMask);
            if ((address != blkAddress) || (size != blkSize))
            {
                // not aligned
                return Result::InvalidParameter;
            }
            rst = media_read(data, address >> _procedConfig._readBlockSizeBits,
                             size >> _procedConfig._readBlockSizeBits);
            if (rst != Result::OK)
            {
                break;
            }
        }
        else if (_config.readMode == BlockMode::Block)
        {
            rst = media_read(data, address, size);
            if (rst != Result::OK)
            {
                break;
            }
        }
        else if (_config.readMode == BlockMode::Wrap)
        {
            uint32_t sizeInBlock, remainSize;
            remainSize = size;

            uint8_t *curDataPtr = (uint8_t *)data;
            do
            {
                sizeInBlock =
                    _config.readBlockSize - (address & (_procedConfig._readBlockSizeMask));
                if (sizeInBlock > remainSize)
                {
                    sizeInBlock = remainSize;
                }

                remainSize -= sizeInBlock;

                rst = media_read(curDataPtr, address, sizeInBlock);

                if (rst != Result::OK)
                {
                    break;
                }

                curDataPtr += sizeInBlock;
                address += sizeInBlock;
            } while (remainSize > 0);

            rst = Result::OK;
            break;
        }
        else
        {
            rst = Result::NotSupport;
            break;
        }
    } while (0);
    return rst;
};
Result Block::write(void *data, uint32_t address, uint32_t size)
{
    Result rst;

    do
    {
        if (_config.writeMode == BlockMode::Blockwise)
        {
            uint32_t blkAddress = address & ~(_procedConfig._writeBlockSizeMask);
            uint32_t blkSize = size & ~(_procedConfig._writeBlockSizeMask);
            if ((address != blkAddress) || (size != blkSize))
            {
                // not aligned
                rst = Result::InvalidParameter;
                break;
            }
        }
        if (_config.needEraseBeforeWrite)
        {
            uint8_t *buffer = this->_buffer.data;

            uint32_t wRemainSize = size;
            uint32_t wAddr = address;

            uint8_t *wData = (uint8_t *)data;
            uint32_t erBlkSize = max(_config.eraseBlockSize, _config.readBlockSize);
            uint32_t erBlkMask = erBlkSize - 1;
            do
            {
                uint32_t erBlkAddr = wAddr & ~erBlkMask;
                uint32_t wPosInBlk = wAddr & erBlkMask;
                uint32_t wSizeInBlk = min(wRemainSize, erBlkSize - wPosInBlk);

                if ((erBlkAddr != wAddr) || (wRemainSize < erBlkSize))
                {
                    // address not aligned to erBlock or tail fragment.
                    // read->memcpy->erase->write.
                    rst = read(buffer, erBlkAddr, erBlkSize); // read entire block
                    if (rst != Result::OK)
                    {
                        break;
                    }
                    memcpy((void *)(buffer + wPosInBlk), (const void *)wData, wSizeInBlk);
                    rst = erase(erBlkAddr, erBlkSize);
                    if (rst != Result::OK)
                    {
                        break;
                    }
                    rst = _write_directly(buffer, erBlkAddr, erBlkSize); // write entire block
                    if (rst != Result::OK)
                    {
                        break;
                    }
                }
                else
                {
                    // address aligned to erBlock. middle entire blocks. directly
                    // erase->write.
                    uint32_t blkCount = wRemainSize / erBlkSize;
                    rst = erase(erBlkAddr, erBlkSize * blkCount);
                    if (rst != Result::OK)
                    {
                        break;
                    }
                    rst =
                        _write_directly(wData, erBlkAddr, erBlkSize * blkCount);
                    if (rst != Result::OK)
                    {
                        break;
                    }
                }

                wAddr += wSizeInBlk;
                wData += wSizeInBlk;
                wRemainSize -= wSizeInBlk;
            } while (wRemainSize > 0);
            return Result::OK;
        }
        else
        {
            rst = _write_directly(data, address, size);
            if (rst != Result::OK)
            {
                break;
            }
        }
    } while (0);

    return rst;
};
Result Block::erase(uint32_t address, uint32_t size)
{
    Result rst;

    do
    {
        // TODO: simplfy these mode.
        if (_config.eraseMode == BlockMode::RandomBlock)
        {
            rst = media_erase(address, size);
            if (rst != Result::OK)
            {
                break;
            }
        }
        else if (_config.eraseMode == BlockMode::Random)
        {
            rst = media_erase(address, size);
            if (rst != Result::OK)
            {
                break;
            }
        }
        else if (_config.eraseMode == BlockMode::Block)
        {
            rst = media_erase(address, size);
            if (rst != Result::OK)
            {
                break;
            }
        }
        else if (_config.eraseMode == BlockMode::Blockwise)
        {
            uint32_t blkAddress = address & ~(_procedConfig._eraseBlockSizeMask);
            uint32_t blkSize = size & ~(_procedConfig._eraseBlockSizeMask);
            if ((address != blkAddress) || (size != blkSize))
            {
                // not aligned
                rst = Result::InvalidParameter;
                break;
            }
            rst = media_erase(address >> (_procedConfig._eraseBlockSizeBits),
                              size >> (_procedConfig._eraseBlockSizeBits));
            if (rst != Result::OK)
            {
                break;
            }
        }
        else if (_config.eraseMode == BlockMode::Wrap)
        {
            rst = Result::OK;
        }
        else
        {
            rst = Result::NotSupport;
        }
    } while (0);

    return rst;
};

Result Block::_write_directly(void *data, uint32_t address, uint32_t size)
{
    Result rst;
    if (_config.writeMode == BlockMode::Random)
    {
        rst = media_write(data, address, size);
        if (rst != Result::OK)
        {
            return rst;
        }
    }
    else if (_config.writeMode == BlockMode::Blockwise)
    {
        rst = media_write(data, address >> (_procedConfig._writeBlockSizeBits),
                          size >> (_procedConfig._writeBlockSizeBits));
        if (rst != Result::OK)
        {
            return rst;
        }
    }
    else if (_config.writeMode == BlockMode::Block)
    {
        rst = media_write(data, address, size);
        if (rst != Result::OK)
        {
            return rst;
        }
    }
    else if (_config.writeMode == BlockMode::Wrap)
    {

        uint32_t sizeInBlock, remainSize;
        remainSize = size;

        uint8_t *curDataPtr = (uint8_t *)data;
        do
        {
            sizeInBlock = _config.writeBlockSize - (address & _procedConfig._writeBlockSizeMask);
            if (sizeInBlock > remainSize)
            {
                sizeInBlock = remainSize;
            }
            rst = media_write(curDataPtr, address, sizeInBlock);
            if (rst != Result::OK)
            {
                return rst;
            }

            remainSize -= sizeInBlock;
            curDataPtr += sizeInBlock;
            address += sizeInBlock;
        } while (remainSize > 0);

        return Result::OK;
    }

    return Result::NotSupport;
};

} // namespace wibot::accessor
