#include "../inc/common/io_chain.h"

#define LOG_MODULE "cc"
#define LOG_LEVEL LOG_LEVEL_INFO
#include "log.h"

static void _op_chain_send_frame(IoChain *ioChain);

static BOOL _io_chain_is_busy(IoChain *ioChain)
{
    ULONG actualFlags;
    return tx_event_flags_get(&ioChain->events, IO_CHAIN_EVENT_CMD_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS;
};

static inline void _io_chain_tx_rx_cplt(SpiWithPinsDevice *device)
{
    IoChain *cmd = (IoChain *)device->base.parent;
    cmd->_curIoChainIndex++;
    _op_chain_send_frame(cmd);
};

static void _io_chain_error(DeviceBase *device, DEVICE_STATUS error)
{
    IoChain *cmd = (IoChain *)device->parent;
    cmd->hasError = 1;
    if (_io_chain_is_busy(cmd))
    {
        EVENTS_RESET_FLAGS(cmd->events, IO_CHAIN_EVENT_CMD_BUSY);
        EVENTS_SET_FLAGS(cmd->events, IO_CHAIN_EVENT_CMD_COMPLETE);
    }

    if (cmd->onError)
    {
        cmd->onError((IoChain *)cmd, error);
    }
};

static void _op_chain_send_frame(IoChain *ioChain)
{
    if (ioChain->_curIoChainIndex < ioChain->_curIoChainFrameSize)
    {
        IoChainFrame frame = ioChain->_curIochainFrame[ioChain->_curIoChainIndex];

        if (frame.statusBits.isRead)
        {
            LOG_D("CMD-SF-A-W:st %u", ioChain->_curIoChainIndex);
            spi_with_pins_device_tx(ioChain->device, frame.statusBits.isData, frame.buffer.data, frame.buffer.size, frame.statusBits.dataBits);
            LOG_D("CMD-SF-A-W:ed %u", ioChain->_curIoChainIndex);
        }
        else
        {
            LOG_D("CMD-SF-A-R:st %u", ioChain->_curIoChainIndex);
            spi_with_pins_device_rx(ioChain->device, frame.statusBits.isData, frame.buffer.data, frame.buffer.size, frame.statusBits.dataBits, frame.statusBits.dummyCycles);
            LOG_D("CMD-SF-A-R:ed %u", ioChain->_curIoChainIndex);
        }
    }
    else
    {
        LOG_D("CMD-SF-E:");
        ioChain->hasError = 0;
        EVENTS_SET_FLAGS(ioChain->events, IO_CHAIN_EVENT_CMD_COMPLETE);
        EVENTS_RESET_FLAGS(ioChain->events, IO_CHAIN_EVENT_CMD_BUSY);
    }
};

DEVICE_STATUS io_chain_create(IoChain *ioChain, SpiWithPinsDevice *device)
{
    ioChain->device = device;
    tx_event_flags_create(&(ioChain->events), "command");
    ioChain->hasError = 0;
    ioChain->_curIochainFrame = NULL;
    ioChain->_curIoChainFrameSize = 0;
    ioChain->_curIoChainIndex = -1;
    ioChain->onError = NULL;

    _spi_with_pins_device_register(device, ioChain,
                                   &_io_chain_tx_rx_cplt,
                                   &_io_chain_tx_rx_cplt,
                                   &_io_chain_error);
    return DEVICE_STATUS_OK;
};

void _io_chain_register(IoChain *ioChain, IoChainEventHandlerFuncType onError)
{
    ioChain->onError = onError;
};

DEVICE_STATUS io_chain_send(IoChain *ioChain, IoChainFrame *ioChainFrame, uint32_t size)
{
    if (_io_chain_is_busy(ioChain))
    {
        return DEVICE_STATUS_BUSY;
    }

    ioChain->_curIochainFrame = ioChainFrame;
    ioChain->_curIoChainIndex = 0;
    ioChain->_curIoChainFrameSize = size;

    EVENTS_CLEAR_FLAGS(ioChain->events);

    EVENTS_SET_FLAGS(ioChain->events, IO_CHAIN_EVENT_CMD_BUSY);
    LOG_D("CMD-SND-CMD");
    _op_chain_send_frame(ioChain);

    return DEVICE_STATUS_OK;
};

DEVICE_STATUS io_chain_cplt_wait(IoChain *ioChain, ULONG timeout)
{
    ULONG actualFlags;
    tx_event_flags_get(&ioChain->events, IO_CHAIN_EVENT_CMD_COMPLETE, TX_AND_CLEAR, &actualFlags, timeout);
    return ioChain->hasError ? DEVICE_STATUS_GENERAL_ERROR : DEVICE_STATUS_OK;
};
