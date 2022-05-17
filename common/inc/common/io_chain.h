#ifndef ___IO_CHAIN_H__
#define ___IO_CHAIN_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "device.h"
#include "spi_with_pins_device.h"
#include "ringbuffer.h"
#include "os.h"
#include "buffer.h"

#define IO_CHAIN_READY 0x01

    typedef enum IO_CHAIN_SELECT_PIN_MODE
    {
        IO_CHAIN_SELECT_PIN_MODE_SELECT = 0x00,
        IO_CHAIN_SELECT_PIN_MODE_UNSELECT = 0x01,
    } IO_CHAIN_SELECT_PIN_MODE;

    typedef enum IO_CHAIN_DATACMD_PIN_MODE
    {
        IO_CHAIN_DATACMD_PIN_MODE_CMD = 0x00,
        IO_CHAIN_DATACMD_PIN_MODE_DATA = 0x01,

    } IO_CHAIN_DATACMD_PIN_MODE;

    typedef enum IO_CHAIN_READWRITE_PIN_MODE
    {
        IO_CHAIN_READWRITE_PIN_MODE_WRITE = 0x00,
        IO_CHAIN_READWRITE_PIN_MODE_READ = 0x01,

    } IO_CHAIN_READWRITE_PIN_MODE;

    typedef struct IoChainFrame
    {
        Buffer buffer;
        union
        {
            uint8_t flags;
            struct
            {
                uint8_t isRead : 1;
                uint8_t isData : 1;
                //uint8_t csNotBreak : 1;
                uint8_t dataBits : 2;
                uint8_t dummyCycles : 4;
            } statusBits;
        };

    } IoChainFrame;

    struct IoChain;
    typedef void (*IoChainEventHandlerFuncType)(struct IoChain *opChain, OP_RESULT error);

    typedef struct IoChain
    {
        SpiWithPinsDevice *device;
        DRIVER_EVENTS events;
        uint32_t hasError;
        IoChainEventHandlerFuncType onError;

        IoChainFrame *_curIochainFrame;
        uint32_t _curIoChainFrameSize;
        uint32_t _curIoChainIndex;

    } IoChain;

    OP_RESULT io_chain_create(IoChain *opChain, SpiWithPinsDevice *device);

    void _io_chain_register(IoChain *opChain, IoChainEventHandlerFuncType onError);

    OP_RESULT io_chain_send(IoChain *opChain, IoChainFrame *commandFrame, uint32_t size);
    OP_RESULT io_chain_cplt_wait(IoChain *opChain, ULONG timeout);

#ifdef __cplusplus
}
#endif

#endif // ___IO_CHAIN_H__