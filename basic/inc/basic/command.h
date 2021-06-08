#ifndef ___COMMAND_H__
#define ___COMMAND_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "basic/device.h"
#include "basic/packet_io_device.h"
#include "basic/ring_buffer8.h"
#include "tx_api.h"
#include "basic/buffer.h"
#include "basic/pin.h"

#define COMMAND_BASE_EVENT_TX_BUSY 0x01
#define COMMAND_BASE_EVENT_TX_COMPLETE 0x02

#define COMMAND_BASE_EVENT_RX_BUSY 0x04
#define COMMAND_BASE_EVENT_RX_COMPLETE 0x08

#define COMMAND_BASE_EVENT_CMD_BUSY 0x10
#define COMMAND_BASE_EVENT_CMD_COMPLETE 0x20

#define COMMAND_BASE_CS_ENABLE(cm) \
    if (cm.csPin != NULL)          \
    Pin_Set(cm.csPin, !cm.csPinCfg)
#define COMMAND_BASE_CS_DISABLE(cm) \
    if (cm.csPin != NULL)           \
    Pin_Set(cm.csPin, cm.csPinCfg)
#define COMMAND_BASE_DC_SET(cm, isCmd) \
    if (cm.dcPin != NULL)              \
    Pin_Set(cm.dcPin, (isCmd) ^ (cm.dcPinCfg))
#define COMMAND_BASE_RW_SET(cm, isWrite) \
    if (cm.rwPin != NULL)                \
    Pin_Set(cm.rwPin, (isWrite) ^ (cm.rwPinCfg))

#define COMMAND_BASE_RETURN_IF_BUSY(command)                                                                             \
    ULONG actualFlags;                                                                                                   \
    if (tx_event_flags_get(&command.events, COMMAND_BASE_EVENT_CMD_BUSY, TX_OR, &actualFlags, TX_NO_WAIT) == TX_SUCCESS) \
    {                                                                                                                    \
        return DEVICE_STATUS_BUSY;                                                                                       \
    }

    typedef enum COMMAND_SELECT_PIN_MODE
    {
        COMMAND_SELECT_PIN_MODE_SELECT = 0x00,
        COMMAND_SELECT_PIN_MODE_UNSELECT = 0x01,
    } COMMAND_SELECT_PIN_MODE;

    typedef enum COMMAND_DATACMD_PIN_MODE
    {
        COMMAND_DATACMD_PIN_MODE_CMD = 0x00,
        COMMAND_DATACMD_PIN_MODE_DATA = 0x01,

    } COMMAND_DATACMD_PIN_MODE;

    typedef enum COMMAND_READWRITE_PIN_MODE
    {
        COMMAND_READWRITE_PIN_MODE_WRITE = 0x00,
        COMMAND_READWRITE_PIN_MODE_READ = 0x01,

    } COMMAND_READWRITE_PIN_MODE;

#define COMMAND_ADDRESS_SIZE_8BIT 0x00;
#define COMMAND_ADDRESS_SIZE_16BIT 0x01;
#define COMMAND_ADDRESS_SIZE_24BIT 0x02;
#define COMMAND_ADDRESS_SIZE_32BIT 0x03;

    typedef struct CommandBase
    {
        PacketIoDevice *device;

        Pin *csPin;
        Pin *rwPin;
        Pin *dcPin;
        COMMAND_SELECT_PIN_MODE csPinCfg; //high represent to cfg's value
        COMMAND_READWRITE_PIN_MODE rwPinCfg;
        COMMAND_DATACMD_PIN_MODE dcPinCfg;
        TX_EVENT_FLAGS_GROUP events;
        uint32_t hasError;
        void (*onError)(struct CommandBase *commandMaster);
    } CommandBase;

    typedef struct CommandFrame
    {
        Buffer buffer;
        union
        {
            uint8_t flags;
            struct
            {
                uint8_t isWrite : 1;
                uint8_t isCmd : 1;
                //uint8_t csNotBreak : 1;
                uint8_t dataBits : 2;
                uint8_t dummyCycles : 4;
            } statusBits;
        };

    } CommandFrame;

    typedef struct CommandMaster
    {
        CommandBase base;

        CommandFrame *_curCommand;
        uint32_t _curCommandFrameSize;
        uint32_t _curCommandFrameIndex;

    } CommandMaster;

    typedef struct SimpleCommand
    {
        CommandBase base;
        void *data;
        uint32_t commandId;
        uint32_t address;
        uint16_t dataSize;
        union
        {
            uint8_t flags;
            struct
            {
                uint8_t isWrite : 1;
                uint8_t hasAddress : 1;
                uint8_t commandBits : 2;
                uint8_t addressBits : 2;
                uint8_t dataBits : 2;
                uint8_t dummyCycles : 4;
            } flagBits;
        };
        uint8_t _phase; // 0 = init, 1 = cmd, 2 = addr, 3 = data, 4 = end;

    } SimpleCommand;

    DEVICE_STATUS command_base_create(CommandBase *commandBase, PacketIoDevice *device,
                                      Pin *csPin, COMMAND_SELECT_PIN_MODE csCfg,
                                      Pin *rwPin, COMMAND_READWRITE_PIN_MODE rwCfg,
                                      Pin *dcPin, COMMAND_DATACMD_PIN_MODE dcCfg);

    uint8_t CommandBase_IsBusy(CommandBase *commandBase);

    DEVICE_STATUS CommandBase_WaitForComplete(CommandBase *commandBase, ULONG timeout);

    DEVICE_STATUS command_master_create(CommandMaster *commandMaster, PacketIoDevice *device,
                                        Pin *csPin, COMMAND_SELECT_PIN_MODE csCfg,
                                        Pin *rwPin, COMMAND_READWRITE_PIN_MODE rwCfg,
                                        Pin *dcPin, COMMAND_DATACMD_PIN_MODE dcCfg);

    DEVICE_STATUS CommandMaster_SendCommandSync(CommandMaster *commandMaster, CommandFrame *command, uint32_t size);

    DEVICE_STATUS CommandMaster_SendCommandAsync(CommandMaster *commandMaster, CommandFrame *command, uint32_t size);

    DEVICE_STATUS simple_command_create(SimpleCommand *simpleCommand, PacketIoDevice *device,
                                        Pin *csPin, COMMAND_SELECT_PIN_MODE csCfg,
                                        Pin *rwPin, COMMAND_READWRITE_PIN_MODE rwCfg,
                                        Pin *dcPin, COMMAND_DATACMD_PIN_MODE dcCfg);
    DEVICE_STATUS SimpleCommand_SendCommandSync(SimpleCommand *simpleCommand);

    DEVICE_STATUS SimpleCommand_SendCommandAsync(SimpleCommand *simpleCommand);

#ifdef __cplusplus
}
#endif

#endif // ___COMMAND_H__