#include "common/shared.hpp"

using namespace Windwolf::Common;

WaitHandle::WaitHandle(OsSync &sync) : _sync(sync), _status(WAIT_STATUS::INIT) {
}

void WaitHandle::Start() {
    this->_status = WAIT_STATUS::WAITING;
};

void WaitHandle::Wait() {
    this->_sync.Wait(-1);
};

void WaitHandle::Finish() {
    this->_status = WAIT_STATUS::DONE;
    this->_sync.Set();
};

void WaitHandle::Cancel() {
    _status = WAIT_STATUS::CANCELED;
    this->_sync.Set();
};

void WaitHandle::Error() {
    _status = WAIT_STATUS::ERROR;
    this->_sync.Set();
};


void *WaitHandle::GetPayload() {
    return _payload;
};

void WaitHandle::SetPayload(void *payload) {
    _payload = payload;
};

WAIT_STATUS WaitHandle::GetStatus() {
    return _status;
};
