#pragma once

#ifndef __common_hpp__
#define __common_hpp__

#include "stdint.h"

#define size_t uint32_t

//static inline bool soft_assert(bool expr) {
//    return expr;
//}; // TODO: implement

namespace windwolf::common {

    enum class DEVICE_STATUS {

        HARDWARE_ERROR = -3,
        ARGUMENT_ERROR = -2,
        GENERAL_ERROR = -1,

        OK = 0,
        NOT_NECESSARY = 1,
        NOT_READY = 2,
        BUSY = 3,
        NOT_SUPPORT = 4,
    };

    template<typename T>
    struct Buffer {
    private:
        T *_begin;
        T *_end;

    public:
        using iterator = T *;
        using const_iterator = const T *;

        Buffer(T *begin, size_t length) : _begin(begin), _end(begin + length) {}

        Buffer(T *begin, T *end) : _begin(begin), _end(end) {}

        Buffer() : _begin(nullptr), _end(nullptr) {}

        template<size_t I>
        Buffer(T (&begin)[I]) : Buffer(begin, I) {}

        Buffer &operator+=(size_t num) {
            if (!soft_assert(num <= size())) {
                num = size();
            }
            _begin += num;
            return *this;
        }

        Buffer operator++(int) {
            Buffer result = *this;
            *this += 1;
            return result;
        }

        T &operator*() { return *_begin; }

        Buffer take(size_t num) const {
            if (!soft_assert(num <= size())) {
                num = size();
            }
            Buffer result = {_begin, num};
            return result;
        }

        Buffer skip(size_t num, size_t *processed_bytes = nullptr) const {
            if (!soft_assert(num <= size())) {
                num = size();
            }
            if (processed_bytes)
                (*processed_bytes) += num;
            return {_begin + num, _end};
        }

        size_t size() const { return _end - _begin; }

        bool empty() const { return size() == 0; }

        T *&begin() { return _begin; }

        T *&end() { return _end; }

        T *const &begin() const { return _begin; }

        T *const &end() const { return _end; }

        T &front() const { return *begin(); }

        T &back() const { return *(end() - 1); }

        T &operator[](size_t idx) { return *(begin() + idx); }
    };

    template<typename T>
    struct Buffer2 {
        T *begin;
        uint32_t size;
        uint32_t count;

        Buffer2() : Buffer2(nullptr, 0, 0) {

        };

        Buffer2(T *begin, uint32_t size) : Buffer2(begin, size, 0) {

        };

        Buffer2(T *begin, uint32_t size, uint32_t count) : begin(begin), size(size), count(count) {

        };

        Buffer2(const Buffer2 &value) {
            begin = value.begin;
            size = value.size;
            count = value.count;
        };

        Buffer2(const Buffer2 &&value) noexcept {
            begin = value.begin;
            size = value.size;
            count = value.count;
            value.begin = nullptr;
            value.size = 0;
            value.count = 0;
        };

        Buffer2<T> &operator=(const Buffer2<T> &value) {
            begin = value.begin;
            size = value.size;
            count = value.count;
            return *this;
        }

    };

    template<typename TRet, typename... TArgs>
    class Callback {
    private:
        TRet (*_function)(void *context, TArgs...);

        void *_context;

    public:
        Callback(TRet (*function)(TArgs...), void *context) : _function(function), _context(context) {};

        TRet invoke(TArgs... args) {
            return (*_function)(_context, args...);
        };
    };

} // namespace windwolf::common

#endif // __common_hpp__
