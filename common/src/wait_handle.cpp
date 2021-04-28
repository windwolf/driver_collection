#include "common/shared.hpp"

using namespace Windwolf::Common;

WaitHandle::WaitHandle(OsEvent &sync) : _sync(sync) {
}

void WaitHandle::Wait() {
    this->_sync.Get(-1);
};

void WaitHandle::Finish() {
    this->_sync.Set();
};

void WaitHandle::Cancel() {
    //TODO:
    this->_sync.Set();
};

void WaitHandle::Error() {
    //TODO:
    this->_sync.Set();
};


void *WaitHandle::GetPayload() {
    return _payload;
};

void WaitHandle::SetPayload(void *payload) {
    _payload = payload;
};

WaitHandle::WAIT_STATUS WaitHandle::GetStatus() {
    if (_sync.Peek()) {
        return WAIT_STATUS::SET;
    } else {
        return WAIT_STATUS::RESET;
    }
};

