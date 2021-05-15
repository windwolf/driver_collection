#include "stdint.h"
#include "common/ring_buffer.hpp"
#include "stdlib.h"
#include "string.h"

using namespace windwolf::common;

#define POINTER_ADD(base, offset) ((base) + (offset))

template <typename T>
RingBuffer<T>::RingBuffer(T *data, uint32_t capacity)
{
    _size = capacity;
    _statusBits.overflowed = 0;
    _data = data;
    _head = 0;
    _count = 0;
    _tail = 0;
}

template <typename T>
void RingBuffer<T>::RegisterOperationNotify(RingBuffer::OperationNotifyCallback callback)
{
    this->_callback = callback;
}

template <typename T>
inline bool RingBuffer<T>::IsFull()
{
    return _count == _size;
}

template <typename T>
inline bool RingBuffer<T>::IsEmpty()
{
    return _count == 0;
}

template <typename T>
inline bool RingBuffer<T>::HasEnoughSpace(uint32_t length)
{
    return (_size - _count) >= length;
}

template <typename T>
inline uint32_t RingBuffer<T>::GetDataLength()
{
    return _count;
}

template <typename T>
inline T *RingBuffer<T>::GetHeadPointer()
{
    return POINTER_ADD(_data, _head);
}

template <typename T>
inline T *RingBuffer<T>::GetTailPointer()
{
    return POINTER_ADD(_data, _tail);
}

template <typename T>
bool RingBuffer<T>::SyncHead(uint32_t newHead)
{
    //uint32_t tail = _tail;
    uint32_t size = _size;
    uint32_t count = _count;
    uint32_t head = _head;
    int32_t headDiff = newHead - head;
    uint32_t length = headDiff >= 0 ? headDiff : headDiff + size;
    uint8_t isEnough = (size - count) >= length;

    count += length;
    head = newHead;

    if (!isEnough)
    {
        _statusBits.overflowed = 1;
        count = size;
        _tail = newHead;
    }
    _count = count;
    _head = newHead;

    if (_callback != NULL)
    {
        _callback(this, OP_TYPE::OP_TYPE_SYNC_HEAD);
    }
    return true;
}

template <typename T>
bool RingBuffer<T>::SyncTail(uint32_t newTail)
{
    uint32_t tail = _tail;
    uint32_t count = _count;
    int32_t tailDiff = newTail - tail;
    uint32_t length = tailDiff >= 0 ? tailDiff : tailDiff + _size;

    if (count < length)
    {
        return false;
    }

    _count -= length;
    _tail = newTail;

    if (_callback != NULL)
    {
        _callback(this, OP_TYPE::OP_TYPE_SYNC_TAIL);
    }
    return true;
}

template <typename T>
int32_t RingBuffer<T>::Enqueue(T *data, uint32_t length, bool allowCoverTail)
{
    uint32_t head = _head;
    uint32_t tail = _tail;
    uint32_t size = _size;
    uint32_t count = _count;
    uint32_t countFormHeadToBufferEnd = size - head;
    uint32_t rtnLength = length;
    uint8_t isEnough = (size - count) >= length;
    if (!isEnough)
    {
        if (allowCoverTail)
        {
            if (length > size)
            {
                // If data length larger than buffer size, the head part of the data will be covered by the tail of the data,
                // only last piece of data which length equals to buffer size will be enqueued to buffer actually.
                data = POINTER_ADD(data, (length - size));
                length = size;
            }
            _statusBits.overflowed = 1;
        }
        else
        {
            // If tail coverage is not allowed, the max length of the data to be enqueued is the rest empty room of the buffer.
            length = (size - count);
            rtnLength = length;
        }
    }

    if (countFormHeadToBufferEnd >= length)
    {
        memcpy(POINTER_ADD(_data, head), data, length);
        head += length;
        head = head == size ? 0 : head;
        count += length;
    }
    else
    {

        memcpy(POINTER_ADD(_data, head), data, countFormHeadToBufferEnd);
        memcpy(_data, POINTER_ADD(data, countFormHeadToBufferEnd), (length - countFormHeadToBufferEnd));

        head = length - countFormHeadToBufferEnd;
        count += length;
    }

    if (!isEnough)
    {
        count = size;
        tail = head;
        _tail = tail;
    }

    _count = count;
    _head = head;

    if (_callback != NULL)
    {
        _callback(this, OP_TYPE::OP_TYPE_ENQUEUE);
    }
    return rtnLength;
}

template <typename T>
int32_t RingBuffer<T>::Dequeue(T *buffer, uint32_t length)
{
    uint32_t tail = _tail;
    uint32_t size = _size;
    uint32_t count = _count;
    uint32_t countFormTailToBufferEnd = size - tail;
    if (count < length)
    {
        length = count;
    }

    if (countFormTailToBufferEnd >= length)
    {
        memcpy(buffer, POINTER_ADD(_data, tail), length);
        tail += length;
        tail = tail == size ? 0 : tail;
        count -= length;
    }
    else
    {
        memcpy(buffer, POINTER_ADD(_data, tail), countFormTailToBufferEnd);
        memcpy(POINTER_ADD(buffer, countFormTailToBufferEnd), _data, (length - countFormTailToBufferEnd));

        tail = length - countFormTailToBufferEnd;
        count -= length;
    }

    _count = count;
    _tail = tail;
    _statusBits.overflowed = 0;

    if (_callback != NULL)
    {
        _callback(this, OP_TYPE::OP_TYPE_DEQUEUE);
    }
    return length;
}

template <typename T>
bool RingBuffer<T>::PeekToEnd(T **buffer, uint32_t *length)
{
    uint32_t tail = _tail;
    uint32_t size = _size;
    uint32_t count = _count;
    uint32_t countFormTailToBufferEnd = size - tail;

    *buffer = POINTER_ADD(_data, _tail);
    *length = count < countFormTailToBufferEnd ? count : countFormTailToBufferEnd;

    if (_callback != NULL)
    {
        _callback(this, OP_TYPE::OP_TYPE_PEEK_TO_END);
    }
    return true;
}

template <typename T>
uint32_t RingBuffer<T>::GetBufferSize()
{
    return _size;
}

template <typename T>
T *RingBuffer<T>::GetBuffer()
{
    return _data;
}

template <typename T>
RingBuffer<T>::RingBuffer(Buffer2<T> buf) : RingBuffer(buf.begin, buf.size)
{
}
