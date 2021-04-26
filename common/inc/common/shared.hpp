#pragma once

#ifndef __common_hpp__
#define __common_hpp__

#include "stdint.h"
#include "common\os.hpp"

#define size_t uint32_t


#define CONVERT_REFERENCE(var, type) (*((type*)&var))

//static inline bool soft_assert(bool expr) {
//    return expr;
//}; // TODO: implement



namespace Windwolf::Common {

    enum class DEVICE_STATUS {
        HARDWARE_ERROR = -3,
        ARGUMENT_ERROR = -2,
        GENERAL_ERROR = -1,

        OK = 0,
        NOT_NECESSARY = 1,
        NOT_READY = 2,
        BUSY = 3,

    };

    template<typename T>
    struct Buffer {
    private:
        T *_begin;
        T *_end;

    public:
        using iterator = T *;
        using
        const_iterator =
        const T *;

        Buffer(T *begin, size_t
        length) : _begin(begin), _end(begin + length) {}

        Buffer(T *begin, T *end) : _begin(begin), _end(end) {}

        Buffer() : _begin(nullptr), _end(nullptr) {}

        template<size_t
        I>
        Buffer(T(&begin)[I]) : Buffer(begin, I) {}

        // Buffer(std::vector<typename std::remove_const<T>::type> &vector)
        //     : generic_bufptr_t(vector.data(), vector.size()) {}

        // Buffer(const std::vector<typename std::remove_const<T>::type> &vector)
        //     : generic_bufptr_t(vector.data(), vector.size()) {}

        // Buffer(const generic_bufptr_t<typename std::remove_const<T>::type> &other)
        //     : generic_bufptr_t(other._begin, other._end) {}

        Buffer &operator+=(size_t
                           num) {
            if (!soft_assert(num <= size())) {
                num = size();
            }
            _begin += num;
            return *this;
        }

        Buffer
        operator++(
                int) {
            Buffer result = *this;
            *this += 1;
            return result;
        }

        T &operator*() {
            return *_begin;
        }

        Buffer
        take(size_t
             num) const {
            if (!soft_assert(num <= size())) {
                num = size();
            }
            Buffer result = {_begin, num};
            return result;
        }

        Buffer
        skip(size_t
             num, size_t *processed_bytes = nullptr) const {
            if (!soft_assert(num <= size())) {
                num = size();
            }
            if (processed_bytes)
                (*processed_bytes) += num;
            return {_begin + num, _end};
        }

        size_t size() const {
            return _end - _begin;
        }

        bool empty() const {
            return size() == 0;
        }

        T *&begin() { return _begin; }

        T *&end() { return _end; }

        T *const &begin() const { return _begin; }

        T *const &end() const { return _end; }

        T &front() const { return *begin(); }

        T &back() const { return *(end() - 1); }

        T &operator[](size_t
                      idx) { return *(begin() + idx); }
    };

    using BufferCU8 = Buffer<const uint8_t>;
    using BufferU8 = Buffer<uint8_t>;

//    template<typename TRet, typename... TArgs>
//    class Callback {
//    private:
//        void *_context;
//
//        TRet (*_callback)(void *, TArgs...);
//
//    public:
//        Callback() : _context(nullptr), _callback(nullptr) {};
//
//        Callback(TRet (*callback)(void *, TArgs...), void *context)
//                : _callback(callback), _context(context) {
//        }
//
//        void SetContext(void *context) {
//            _context = context;
//        }
//
//        void SetFuncPtr(TRet (*callback)(void *, TArgs...)) {
//            _callback = callback;
//        }
//
//        TRet Invoke(TArgs... args) {
//            return _callback(_context, args...);
//        }
//    };

    enum class WAIT_STATUS {
        INIT,
        WAITING,
        DONE,
        CANCELED,
        ERROR,
    };

    class WaitHandle {

    private:
        void *_payload;
        WAIT_STATUS _status;
        OsSync &_sync;

    public:
        WaitHandle(OsSync &sync);

        void Start();

        void Wait();

        void Finish();

        void Cancel();

        void Error();

        void *GetPayload();

        void SetPayload(void *payload);

        WAIT_STATUS GetStatus();
    };

} // namespace Windwolf::Common

#endif // __common_hpp__
