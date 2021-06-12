#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
    typedef enum RINGBUFFER_OPERATION_TYPE
    {
        RINGBUFFER_OPERATION_TYPE_ENQUEUE,
        RINGBUFFER_OPERATION_TYPE_DEQUEUE,
        RINGBUFFER_OPERATION_TYPE_SYNC_HEAD,
        RINGBUFFER_OPERATION_TYPE_SYNC_TAIL,
        RINGBUFFER_OPERATION_TYPE_PEEK_TO_END,
    } RINGBUFFER_OPERATION_TYPE;

    typedef struct RingBuffer
    {
        uint8_t *data;
        uint32_t size;
        uint32_t write; // 指向当前最新的数据
        uint32_t read;
        //uint8_t writeMirror;
        //uint8_t readMirror;
        union
        {
            uint8_t allStatus;
            struct
            {
                uint8_t overflowed : 1;
            } statusBits;
        };

        void (*OperationNotify)(RINGBUFFER_OPERATION_TYPE type);
        // union {
        //     uint8_t allFlags;
        //     struct
        //     {
        //         uint8_t allowCoverTail : 1;
        //     } flagBits;
        // };

    } RingBuffer;

    /**
     * @brief 
     * 
     * @param queue 
     * @param dataPtr 
     * @param maxSize 
     */
    void ringbuffer_create(RingBuffer *queue, uint8_t *dataPtr, uint32_t maxSize);
    uint8_t ringbuffer_is_full(RingBuffer *queue);
    uint8_t ringbuffer_is_empty(RingBuffer *queue);
    uint8_t ringbuffer_space_enough(RingBuffer *queue, uint32_t length);
    uint32_t ringbuffer_mem_size_get(RingBuffer *queue);
    uint32_t ringbuffer_count_get(RingBuffer *queue);
    uint8_t *ringbuffer_write_ptr_get(RingBuffer *queue);
    uint8_t *ringbuffer_read_ptr_get(RingBuffer *queue);
    uint8_t ringbuffer_write_index_sync(RingBuffer *queue, uint32_t newHead);
    uint8_t ringbuffer_read_index_sync(RingBuffer *queue, uint32_t newTail);
    int32_t ringbuffer_write(RingBuffer *queue, uint8_t *valuePtr, uint32_t length, uint8_t allowCoverTail);
    int32_t ringbuffer_read(RingBuffer *queue, uint8_t *valuePtr, uint32_t length);
    uint8_t ringbuffer_peek_to_end(RingBuffer *queue, uint8_t **valuePtr, uint32_t *length);

    void ringbuffer_operation_notify_register(RingBuffer *queue, void (*OperationNotify)(RINGBUFFER_OPERATION_TYPE type));

#ifdef __cplusplus
}
#endif

#endif // __RINGBUFFER_H__