#ifndef __RING_BUFFER_H_
#define __RING_BUFFER_H_

#include "stdint.h"
#include "common/shared.hpp"

namespace windwolf::common {
    template<typename T>
    class RingBuffer {
    public:
        // enum UNIT_SIZE
        // {
        //     UNIT_SIZE_1 = 0,
        //     UNIT_SIZE_2 = 1,
        //     UNIT_SIZE_4 = 2,
        //     UNIT_SIZE_8 = 3,
        // };
        enum OP_TYPE {
            OP_TYPE_ENQUEUE,
            OP_TYPE_DEQUEUE,
            OP_TYPE_SYNC_HEAD,
            OP_TYPE_SYNC_TAIL,
            OP_TYPE_PEEK_TO_END,
        };

        typedef void (*OperationNotifyCallback)(const RingBuffer *const &buffer, OP_TYPE type);

        RingBuffer(T *data, uint32_t capacity);

        RingBuffer(Buffer2 <T> buf);

        bool IsFull();

        bool IsEmpty();

        bool HasEnoughSpace(uint32_t length);

        uint32_t GetDataLength();

        T *GetHeadPointer();

        T *GetTailPointer();

        /**
         * @brief Update the quenes's head. 
         * Used by hardware driver after Rx DMA operation.
         * 
         * @param newHead new head index;
         * @return Whether operation success.
         */
        bool SyncHead(uint32_t newHead);

        /**
         * @brief Update the tail index of the quene. 
         * Used by hardware driver after Tx DMA operation.
         * 
         * @param newTail New Tail index
         * @return Whether operation success.
         */
        bool SyncTail(uint32_t newTail);

        /**
         * @brief Add data to the buffer's head. 
         * If buffer has not enough room for the income data, 
         * and {@allowCoverTail} is true, the data locate on the tail of the buffer will be covered.
         * If the data is larger than whole buffer size, the tail of the data itself will be coverd by
         * the head of the data.
         * 
         * @param data 
         * @param length 
         * @param allowCoverTail allow to cover the tail data 
         * if there is not enough room for the incoming data.
         * @return int32_t The actual length of data that enqueue to the buffer. 
         * Note that actual length maybe larger than buffer capacity, if allowCoverTail is true. 
         */
        int32_t Enqueue(T *data, uint32_t length, bool allowCoverTail);

        /**
         * @brief Fetch data from the end of the buffer. 
         * The room of the fetched data will be mark as emtpy for the next incoming data.
         * 
         * @param buffer buffer to take the dequeue data. 
         * @param length buffer size.
         * @return int32_t The actual length of buffer that fetched from the buffer
         */
        int32_t Dequeue(T *buffer, uint32_t length);

        /**
         * @brief Peek the data from tail to the end of the underline buffer.
         * Usually used by frame parser, to detect the start flag of a new frame.
         * @param buffer the start address of the data.
         * Note that this is the raw buffer address, without any copy to .
         * @param length data length
         * @return Indicate Whether operation success. always true.
         */
        bool PeekToEnd(T **buffer, uint32_t *length);

        /**
         * @brief Register a callback which will be call after buffer operation to buffer.
         * 
         * @param callback 
         */
        void RegisterOperationNotify(OperationNotifyCallback callback);

        T *GetBuffer();

        uint32_t GetBufferSize();

    private:
        T *_data;
        uint32_t _head; // 指向当前最新的数据
        uint32_t _tail;
        uint32_t _size;
        uint32_t _count;
        union {
            uint8_t _allStatus;
            struct {
                // Indicate wether buffer overflow happened.
                // If overflow, the tail of the buffer data will be covered by new data.
                uint8_t overflowed: 1;
            } _statusBits;
        };

        OperationNotifyCallback _callback;
    };


    template
    class RingBuffer<uint8_t>;

    template
    class RingBuffer<uint16_t>;

    template
    class RingBuffer<uint32_t>;

} // namespace windwolf::common

#endif //__RING_BUFFER_H_
