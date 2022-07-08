#include "common/io_chain.h"

#define LOG_MODULE "cc"
#define LOG_LEVEL LOG_LEVEL_INFO
#include "log.h"

static void _op_chain_send_frame(IoChain *ioChain);

static bool _io_chain_is_busy(IoChain *ioChain)
{
    return driver_events_get(&(ioChain->events), IO_CHAIN_READY, DRIVER_EVENTS_OPTION_AND, DRIVER_TIMEOUT_NOWAIT);
};

static inline void _io_chain_tx_rx_cplt(SpiWithPinsDevice *device)
{
    IoChain *cmd = (IoChain *)device->base.parent;
    cmd->_curIoChainIndex++;
    _op_chain_send_frame(cmd);
};

static void _io_chain_error(DeviceBase *device, OP_RESULT error)
{
    IoChain *cmd = (IoChain *)device->parent;
    cmd->hasError = 1;
    if (_io_chain_is_busy(cmd))
    {
        driver_events_set(&(cmd->events), IO_CHAIN_READY);
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
        driver_events_set(&(ioChain->events), IO_CHAIN_READY);
    }
};

OP_RESULT io_chain_create(IoChain *ioChain, SpiWithPinsDevice *device)
{
    ioChain->device = device;
    driver_events_create(&(ioChain->events), "command");
    driver_events_set(&(ioChain->events), IO_CHAIN_READY);
    ioChain->hasError = 0;
    ioChain->_curIochainFrame = NULL;
    ioChain->_curIoChainFrameSize = 0;
    ioChain->_curIoChainIndex = -1;
    ioChain->onError = NULL;

    _spi_with_pins_device_register(device, ioChain,
                                   &_io_chain_tx_rx_cplt,
                                   &_io_chain_tx_rx_cplt,
                                   &_io_chain_error);
    return OP_RESULT_OK;
};

void _io_chain_register(IoChain *ioChain, IoChainEventHandlerFuncType onError)
{
    ioChain->onError = onError;
};

OP_RESULT io_chain_send(IoChain *ioChain, IoChainFrame *ioChainFrame, uint32_t size)
{
    if (_io_chain_is_busy(ioChain))
    {
        return OP_RESULT_BUSY;
    }

    ioChain->_curIochainFrame = ioChainFrame;
    ioChain->_curIoChainIndex = 0;
    ioChain->_curIoChainFrameSize = size;

    driver_events_reset(&(ioChain->events), IO_CHAIN_READY);
    LOG_D("CMD-SND-CMD");
    _op_chain_send_frame(ioChain);

    return OP_RESULT_OK;
};

OP_RESULT io_chain_cplt_wait(IoChain *ioChain, uint32_t timeout)
{
    if (!driver_events_get(&(ioChain->events), IO_CHAIN_READY, DRIVER_EVENTS_OPTION_AND, timeout))
    {
        return OP_RESULT_GENERAL_ERROR;
    }

    return ioChain->hasError ? OP_RESULT_GENERAL_ERROR : OP_RESULT_OK;
};
