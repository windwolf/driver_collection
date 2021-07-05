// #include "common/block_pool.h"

// void data_block_pool_create(DataBlockPool *blockPool, DataBlock *blocks, uint32_t size)
// {
//     ringbuffer_create(&blockPool->blocks, blocks, sizeof(DataBlock), size);
// };

// OP_RESULT data_block_pool_in(DataBlockPool *blockPool, uint8_t *data, uint32_t size)
// {
//     if (ringbuffer32_is_full(&blockPool->blocks))
//     {
//         return OP_RESULT_GENERAL_ERROR;
//     }
//     DataBlock block = {
//         .data = data,
//         .size = size,
//         .completed = 0,
//         .refCount = 0,
//     };
//     ringbuffer_write(&blockPool->blocks, &block, 1, false);
//     return OP_RESULT_OK;
// }
// OP_RESULT data_block_pool_peek(DataBlockPool *blockPool, DataBlock **block)
// {
//     ringbuffer_peek(&blockPool->blocks, block);
//     return OP_RESULT_OK;
// };
// OP_RESULT data_block_pool_out(DataBlockPool *blockPool, DataBlock *block)
// {
//     ringbuffer_read(&blockPool->blocks, block, 1);
//     return OP_RESULT_OK;
// };

// OP_RESULT data_block_pool_lock(DataBlockPool *blockPool, uint32_t beginIndex, uint32_t endIndex)
// {
//     uint32_t curIndex = beginIndex;
//     DataBlock *curBlock;
//     do
//     {
//         curBlock = (DataBlock *)ringbuffer_index_peek(&blockPool->blocks, curIndex);
//         curBlock->refCount++;
//     } while (curIndex > endIndex);
//     return OP_RESULT_OK;
// };
// OP_RESULT data_block_pool_unlock(DataBlockPool *blockPool, uint32_t beginIndex, uint32_t endIndex)
// {
//     uint32_t curIndex = beginIndex;
//     DataBlock *curBlock;
//     do
//     {
//         curBlock = (DataBlock *)ringbuffer_index_peek(&blockPool->blocks, curIndex);
//         curBlock->refCount--;
//     } while (curIndex > endIndex);
//     return OP_RESULT_OK;
// };

// OP_RESULT data_block_pool_release(DataBlockPool *blockPool, uint32_t beginIndex, uint32_t endIndex)
// {
//     return OP_RESULT_OK;
// };
