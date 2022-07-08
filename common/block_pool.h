// #ifndef ___BLOCK_POOL_H__
// #define ___BLOCK_POOL_H__

// #ifdef __cplusplus
// extern "C"
// {
// #endif

// #include "stdint.h"
// #include "shared.h"
// #include "ringbuffer.h"
//     typedef struct BlockIndex
//     {
//         uint32_t blockIndex;
//         uint32_t blockOffset;
//     } BlockIndex;

//     typedef struct DataBlock
//     {
//         uint8_t *data;
//         uint32_t size;
//         uint8_t completed : 1;
//         uint8_t refCount : 7; // Used by parser to indicate the
//     } DataBlock;

//     typedef struct DataBlockPool
//     {
//         RingBuffer blocks;

//     } DataBlockPool;

//     void data_block_pool_create(DataBlockPool *blockPool, DataBlock *blocks, uint32_t size);

//     OP_RESULT data_block_pool_in(DataBlockPool *blockPool, uint8_t *data, uint32_t size);
//     OP_RESULT data_block_pool_peek(DataBlockPool *blockPool, DataBlock **block);
//     OP_RESULT data_block_pool_out(DataBlockPool *blockPool, DataBlock *block);

//     OP_RESULT data_block_pool_lock(DataBlockPool *blockPool, uint32_t beginIndex, uint32_t endIndex);
//     OP_RESULT data_block_pool_unlock(DataBlockPool *blockPool, uint32_t beginIndex, uint32_t endIndex);
//     OP_RESULT data_block_pool_release(DataBlockPool *blockPool, uint32_t beginIndex, uint32_t endIndex);

// #ifdef __cplusplus
// }
// #endif

// #endif // ___BLOCK_POOL_H__