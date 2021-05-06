//
// Created by windwolf on 2021/4/28.
//

#include <common/stream_driver.hpp>

using namespace windwolf::drivers;
using namespace windwolf::common;
using namespace windwolf::os;

static void StreamDriver_Worker(ULONG input) {
    auto dvr = reinterpret_cast<StreamDriver *>(input);

    while (!dvr->_markAsStop) {
        if (dvr->_rxWaitHandle.wait(true)) {
            auto data = dvr->_rxWaitHandle.getPayload();
            dvr->_rxBuffer.Enqueue(data.begin, data.count, true);
        }
    }
}

StreamDriver::StreamDriver(IoDevice &device, OsThread &worker, Buffer2<uint8_t> txBuf,
                           Buffer2<uint8_t> rxBuf, OsEvent &txEvent,
                           OsEvent &rxEvent)
        : _device(device), _worker(worker), _status(Status::initial), _txBuffer(txBuf), _rxBuffer(rxBuf), _txWaitHandle(txEvent),
          _rxWaitHandle(rxEvent), _markAsStop(false) {

}

bool StreamDriver::Close() {
    auto suc = _device.StopRx() == DEVICE_STATUS::OK;
    _worker.stop();
    return true;
}

bool StreamDriver::Open() {

    _worker.start("strdvr", &StreamDriver_Worker, reinterpret_cast<ULONG>(this), 16, 16, 10);
    if (_device.RxAsyncForever(_rxBuffer.GetBuffer(), _rxBuffer.GetBufferSize(), &_rxWaitHandle) != DEVICE_STATUS::OK) {
        return false;
    }
    return true;
}


DEVICE_STATUS StreamDriver::Write(Buffer2<uint8_t> data) {
    return _device.TxAsync(data.begin, data.count, &_txWaitHandle);
}

DEVICE_STATUS StreamDriver::Read(Buffer2<uint8_t> data) {
    // if (_rxBuffer.)
    return DEVICE_STATUS::ARGUMENT_ERROR;
}

