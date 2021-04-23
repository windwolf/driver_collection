//
// Created by windwolf on 2021/4/23.
//

#ifndef STARTUP_DRIVER_H
#define STARTUP_DRIVER_H

enum DEVICE_STATUS {
    DEVICE_STATUS_ARGUMENT_ERROR = -2,
    DEVICE_STATUS_GENERAL_ERROR = -1,

    DEVICE_STATUS_OK = 0,
    DEVICE_STATUS_NOT_NECESSARY = 1,
    DEVICE_STATUS_NOT_READY = 2,
    DEVICE_STATUS_BUSY = 3,

};


typedef struct IoDevice {

    void (*txCallback)(void *context, size_t count);

    void (*rxCallback)(void *context, uint8_t *data, size_t dataSizes);

    DEVICE_STATUS (*Tx)(uint8_t *writeData, uint32_t dataSize);

    DEVICE_STATUS (*Rx)(uint8_t *readBuffer, uint32_t bufferSize);

    DEVICE_STATUS (*TxAsync)(uint8_t *writeData, uint32_t dataSize);

    DEVICE_STATUS (*RxAsync)(uint8_t *readBuffer, uint32_t bufferSize);

    DEVICE_STATUS (*StartRxLoop)(const Buffer <uint8_t> *buffer);

    DEVICE_STATUS (*StopRxLoop());
} IoDeviceTypeDef;


typedef struct StreamDriver {
    IoDevice *device;
} StreamDriverTypeDef;

DEVICE_STATUS StreamDriver_Init(StreamDriver *driver, IoDevice *device) {

}


#endif //STARTUP_DRIVER_H
