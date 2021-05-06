#include "common/wait_handle.hpp"

using namespace windwolf::common;
using namespace windwolf::os;

template<typename T>
WaitHandle<T>::WaitHandle(OsEvent &sync) : _sync(sync), _payload() {
}

template<typename T>
bool WaitHandle<T>::wait(bool reset) {
    this->_sync.get(-1);
    if (reset) {
        _sync.get(-1);
    } else {
        _sync.peek(-1);
    }
    return _flags.value == 0;
};

template<typename T>
void WaitHandle<T>::setDone(T payload) {
    _payload = payload;
    _flags.value = 0;
    this->_sync.set();
};

template<typename T>
void WaitHandle<T>::setCancel() {
    _flags.canceled = 1;
    this->_sync.set();
};

template<typename T>
void WaitHandle<T>::setError() {
    _flags.error = 1;
    this->_sync.set();
}

template<typename T>
T WaitHandle<T>::getPayload() {
    return _payload;
};

//WaitHandle::WAIT_STATUS WaitHandle::GetStatus() {
//    if (_sync.peek()) {
//        return WAIT_STATUS::SET;
//    } else {
//        return WAIT_STATUS::RESET;
//    }
//};
