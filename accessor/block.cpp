#include "block.hpp"

#include "string.h"

#define LOG_MODULE "block"
#include "log.h"

namespace ww::accessor
{

BlockConfig &Block::config_Get()
{
    return _config;
};

Result Block::init()
{
    _process_config();
    return Result_OK;
};
Result Block::deinit()
{
    return Result_OK;
};

Result Block::_process_config()
{
    if (_config.readMode == BlockMode_RandomBlock)
    {
        LOG_E("read should not RANDOM_BLOCK");
        return Result_InvalidParameter;
    }
    if (_config.writeMode == BlockMode_RandomBlock)
    {
        LOG_E("write should not RANDOM_BLOCK");
        return Result_InvalidParameter;
    }
    if (_config.writeBlockSize > _config.eraseBlockSize)
    {
        LOG_E("write block should not be great then erase block size.");
        return Result_InvalidParameter;
    }
    if (_buffer.size < max(_config.readBlockSize, _config.eraseBlockSize))
    {
        return Result_InvalidParameter;
    }

    uint32_t maxBlockSize = (_config.readBlockSize > _config.writeBlockSize)
                                ? _config.readBlockSize
                                : _config.writeBlockSize;
    maxBlockSize = (maxBlockSize > _config.eraseBlockSize)
                       ? maxBlockSize
                       : _config.eraseBlockSize;
    _procedConfig._maxBlockSize = maxBlockSize;

    _procedConfig._readBlockSizeBits = fast_log2(_config.readBlockSize);
    _procedConfig._writeBlockSizeBits = fast_log2(_config.writeBlockSize);
    _procedConfig._eraseBlockSizeBits = fast_log2(_config.eraseBlockSize);
    _procedConfig._readBlockSizeMask = _config.readBlockSize - 1;
    _procedConfig._writeBlockSizeMask = _config.writeBlockSize - 1;
    _procedConfig._eraseBlockSizeMask = _config.eraseBlockSize - 1;
    return Result_OK;
};

Result Block::read(void *data, uint32_t address, uint32_t size)
{
    Result rst;
    if (_config.readMode == BlockMode_Random)
    {
        return media_read(data, address, size);
    }
    else if (_config.readMode == BlockMode_Blockwise)
    {
        uint32_t blkAddress = address & ~(_procedConfig._readBlockSizeMask);
        uint32_t blkSize = size & ~(_procedConfig._readBlockSizeMask);
        if ((address != blkAddress) || (size != blkSize))
        {
            // not aligned
            return Result_InvalidParameter;
        }
        return media_read(data, address >> _procedConfig._readBlockSizeBits,
                          size >> _procedConfig._readBlockSizeBits);
    }
    else if (_config.readMode == BlockMode_Block)
    {
        return media_read(data, address, size);
    }
    else if (_config.readMode == BlockMode_Wrap)
    {
        uint32_t sizeInBlock, remainSize;
        remainSize = size;

        uint8_t *curDataPtr = (uint8_t *)data;
        do
        {
            sizeInBlock = _config.readBlockSize -
                          (address & (_procedConfig._readBlockSizeMask));
            if (sizeInBlock > remainSize)
            {
                sizeInBlock = remainSize;
            }
            rst = media_read(curDataPtr, address, sizeInBlock);

            if (rst != Result_OK)
            {
                return rst;
            }

            remainSize -= sizeInBlock;
            curDataPtr += sizeInBlock;
            address += sizeInBlock;
        } while (remainSize > 0);

        return Result_OK;
    }
    else
    {
        return Result_NotSupport;
    }
};
Result Block::write(void *data, uint32_t address, uint32_t size)
{
    Result rst;
    if (_config.writeMode == BlockMode_Blockwise)
    {
        uint32_t blkAddress = address & ~(_procedConfig._writeBlockSizeMask);
        uint32_t blkSize = size & ~(_procedConfig._writeBlockSizeMask);
        if ((address != blkAddress) || (size != blkSize))
        {
            // not aligned
            return Result_InvalidParameter;
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
                rst = read(buffer, erBlkAddr,
                           erBlkSize); // read entire block
                if (rst != Result_OK)
                {
                    return rst;
                }
                memcpy((void *)(buffer + wPosInBlk), (const void *)wData,
                       wSizeInBlk);
                rst = erase(erBlkAddr, erBlkSize);
                if (rst != Result_OK)
                {
                    return rst;
                }
                rst = _write_directly(buffer, erBlkAddr,
                                      erBlkSize); // write entire block
                if (rst != Result_OK)
                {
                    return rst;
                }
            }
            else
            {
                // address aligned to erBlock. middle entire blocks. directly
                // erase->write.
                uint32_t blkCount = wRemainSize / erBlkSize;
                rst = erase(erBlkAddr, erBlkSize * blkCount);
                if (rst != Result_OK)
                {
                    return rst;
                }
                rst = _write_directly(wData, erBlkAddr, erBlkSize * blkCount);
                if (rst != Result_OK)
                {
                    return rst;
                }
            }

            wAddr += wSizeInBlk;
            wData += wSizeInBlk;
            wRemainSize -= wSizeInBlk;
        } while (wRemainSize > 0);
        return Result_OK;
    }
    else
    {
        return _write_directly(data, address, size);
    }
};
Result Block::erase(uint32_t address, uint32_t size)
{
    // TODO: simplfy these mode.
    if (_config.eraseMode == BlockMode_RandomBlock)
    {
        return media_erase(address, size);
    }
    else if (_config.eraseMode == BlockMode_Random)
    {
        return media_erase(address, size);
    }
    else if (_config.eraseMode == BlockMode_Block)
    {
        return media_erase(address, size);
    }
    else if (_config.eraseMode == BlockMode_Blockwise)
    {
        uint32_t blkAddress = address & ~(_procedConfig._eraseBlockSizeMask);
        uint32_t blkSize = size & ~(_procedConfig._eraseBlockSizeMask);
        if ((address != blkAddress) || (size != blkSize))
        {
            // not aligned
            return Result_InvalidParameter;
        }
        return media_erase(address >> (_procedConfig._eraseBlockSizeBits),
                           size >> (_procedConfig._eraseBlockSizeBits));
    }
    else if (_config.eraseMode == BlockMode_Wrap)
    {
        return Result_OK;
    }
    else
    {
        return Result_NotSupport;
    }
};

Result Block::_write_directly(void *data, uint32_t address, uint32_t size)
{
    Result rst;
    if (_config.writeMode == BlockMode_Random)
    {
        return media_write(data, address, size);
    }
    else if (_config.writeMode == BlockMode_Blockwise)
    {
        return media_write(data, address >> (_procedConfig._writeBlockSizeBits),
                           size >> (_procedConfig._writeBlockSizeBits));
    }
    else if (_config.writeMode == BlockMode_Block)
    {
        return media_write(data, address, size);
    }
    else if (_config.writeMode == BlockMode_Wrap)
    {

        uint32_t sizeInBlock, remainSize;
        remainSize = size;

        uint8_t *curDataPtr = (uint8_t *)data;
        do
        {
            sizeInBlock = _config.writeBlockSize -
                          (address & _procedConfig._writeBlockSizeMask);
            if (sizeInBlock > remainSize)
            {
                sizeInBlock = remainSize;
            }
            rst = media_write(curDataPtr, address, sizeInBlock);
            if (rst != Result_OK)
            {
                return rst;
            }

            remainSize -= sizeInBlock;
            curDataPtr += sizeInBlock;
            address += sizeInBlock;
        } while (remainSize > 0);

        return Result_OK;
    }
    else
    {
        return Result_NotSupport;
    }
}

} // namespace ww::accessor
