#ifndef ___BLOCK_H__
#define ___BLOCK_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "shared.h"
#include "device.h"

    struct Block;

#define BLOCK_MODE uint8_t

/**
 * @brief address random, operate random.
 * 
 */
#define BLOCK_MODE_RANDOM 0x00
/**
 * @brief address random, operate block wrap.
 * 
 */
#define BLOCK_MODE_WRAP 0x01
/**
 * @brief address align to block, operate align to block
 * 
 */
#define BLOCK_MODE_BLOCKWISE 0x02
/**
 * @brief address random, operate align to block
 * 
 */
#define BLOCK_MODE_RANDOM_BLOCK 0x03
/**
 * @brief address block, operate block
 * 
 */
#define BLOCK_MODE_BLOCK 0x04

    /**
    * @brief 
    * sd            = BLOCK = address blockwise,    op blockwise.
    * w25qxx erase  =       = address random,       op blockwise.
    * w25qxx read   =       = address random,       op random.
    * w25qxx write  =       = address random,       op random. wrap.
    */
    typedef struct BlockConfig
    {
        uint32_t readBlockwise : 1;         // read in blockwise.
        uint32_t writeBlockwise : 1;        // write in blockwise.
        uint32_t eraseBlockwise : 1;        // erase in blockwise.
        uint32_t readAddressBlockwise : 1;  // read address in block number.
        uint32_t writeAddressBlockwise : 1; // write address in block number.
        uint32_t eraseAddressBlockwise : 1; // erase address in block number.
        uint32_t needEraseBeforeWrite : 1;  // erase before write.
    } BlockConfig;

    typedef struct Block
    {
        void *instance;

        //BlockConfig config;
        /**
         * @brief final size is power of 2.
         * 
         */
        uint32_t readBlockSize;
        uint32_t writeBlockSize;
        uint32_t eraseBlockSize;

        bool needEraseBeforeWrite;
        BLOCK_MODE readMode;
        BLOCK_MODE writeMode;
        BLOCK_MODE eraseMode;

        Buffer buffer;

        uint32_t _maxBlockSize;
        uint32_t _readBlockSizeBits;
        uint32_t _writeBlockSizeBits;
        uint32_t _eraseBlockSizeBits;

        uint32_t _readBlockSizeMask;
        uint32_t _writeBlockSizeMask;
        uint32_t _eraseBlockSizeMask;

        OP_RESULT(*read)
        (void *instance, void *data, uint32_t num, uint32_t size);
        OP_RESULT(*write)
        (void *instance, void *data, uint32_t num, uint32_t size);
        OP_RESULT(*erase)
        (void *instance, uint32_t num, uint32_t size);
    } Block;

    OP_RESULT block_create(Block *block, void *instance,
                           uint32_t readBlockSize,
                           uint32_t writeBlockSize,
                           uint32_t eraseBlockSize,
                           bool needEraseBeforeWrite,
                           BLOCK_MODE readMode,
                           BLOCK_MODE writeMode,
                           BLOCK_MODE eraseMode,
                           Buffer buffer,
                           OP_RESULT (*read)(void *instance, void *data, uint32_t address, uint32_t size),
                           OP_RESULT (*write)(void *instance, void *data, uint32_t address, uint32_t size),
                           OP_RESULT (*erase)(void *instance, uint32_t address, uint32_t size));
    OP_RESULT block_read(Block *block, void *data, uint32_t address, uint32_t size);
    OP_RESULT block_write(Block *block, void *data, uint32_t address, uint32_t size);
    OP_RESULT block_erase(Block *block, uint32_t address, uint32_t size);
    OP_RESULT _block_write_directly(Block *block, void *data, uint32_t address, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // ___BLOCK_H__