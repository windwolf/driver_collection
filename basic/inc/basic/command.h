#ifndef ___COMMAND_H__
#define ___COMMAND_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "basic/device.h"
#include "basic/ring_buffer8.h"
#include "tx_api.h"
#include "basic/buffer.h"
#include "basic/pin.h"

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

    typedef enum COMMAND_DEVICE_OP_MODE
    {
        COMMAND_DEVICE_OP_MODE_SYNC = 0x1,
        COMMAND_DEVICE_OP_MODE_ASYNC = 0x2,
    } COMMAND_DEVICE_OP_MODE;

    typedef struct CommandMasterDevice
    {
        void *instance;
        COMMAND_DEVICE_OP_MODE opMode;
        void (*Init)(struct CommandMasterDevice *device);
        DEVICE_STATUS(*TxN8)
        (struct CommandMasterDevice *device, uint8_t *data, uint32_t size);
        DEVICE_STATUS(*RxN8)
        (struct CommandMasterDevice *device, uint8_t *buffer, uint32_t size, uint8_t dummyCycleCount);
        DEVICE_STATUS(*TxN8Async)
        (struct CommandMasterDevice *device, uint8_t *data, uint32_t size);
        DEVICE_STATUS(*RxN8Async)
        (struct CommandMasterDevice *device, uint8_t *buffer, uint32_t size, uint8_t dummyCycleCount);

        DEVICE_STATUS(*TxN16)
        (struct CommandMasterDevice *device, uint16_t *data, uint32_t size);
        DEVICE_STATUS(*RxN16)
        (struct CommandMasterDevice *device, uint16_t *buffer, uint32_t size, uint8_t dummyCycleCount);
        DEVICE_STATUS(*TxN16Async)
        (struct CommandMasterDevice *device, uint16_t *data, uint32_t size);
        DEVICE_STATUS(*RxN16Async)
        (struct CommandMasterDevice *device, uint16_t *buffer, uint32_t size, uint8_t dummyCycleCount);
    } CommandMasterDevice;

    typedef struct CommandFrame
    {
        union
        {
            uint8_t flags;
            struct
            {
                uint8_t isWrite : 1;
                uint8_t isCmd : 1;
                uint8_t csNotBreak : 1;
                uint8_t is16Bits : 1;
                uint8_t dummyCycles : 4;
            } statusBits;
        };
        Buffer buffer;
    } CommandFrame;

    typedef struct CommandMaster
    {
        CommandMasterDevice device;
        Pin *csPin;
        Pin *rwPin;
        Pin *dcPin;
        COMMAND_SELECT_PIN_MODE csPinCfg; //high represent to cfg's value
        COMMAND_READWRITE_PIN_MODE rwPinCfg;
        COMMAND_DATACMD_PIN_MODE dcPinCfg;

        TX_EVENT_FLAGS_GROUP events;

        CommandFrame *_curCommand;
        uint32_t _curCommandFrameSize;
        uint32_t _curCommandFrameIndex;
        uint32_t hasError;
        // /**
        //  * @brief tx完成后的回调. @note 回调运行在中断上下文中, 注意控制时间.
        //  *
        //  */
        // void (*onTxComplete)(struct CommandMaster *commandMaster);
        // /**
        //  * @brief rx有值时的回调. @note 回调运行在中断上下文中, 注意控制时间.
        //  *
        //  */
        // void (*onRxComplete)(struct CommandMaster *commandMaster);
        void (*onError)(struct CommandMaster *commandMaster);

    } CommandMaster;

    DEVICE_STATUS CommandMaster_Init(CommandMaster *commandMaster);

    DEVICE_STATUS CommandMaster_ConfigCs(CommandMaster *commandMaster, Pin *csPin, COMMAND_SELECT_PIN_MODE config);

    DEVICE_STATUS CommandMaster_ConfigRw(CommandMaster *commandMaster, Pin *rwPin, COMMAND_READWRITE_PIN_MODE config);

    DEVICE_STATUS CommandMaster_ConfigDc(CommandMaster *commandMaster, Pin *dcPin, COMMAND_DATACMD_PIN_MODE config);

    DEVICE_STATUS CommandMaster_SendCommandSync(CommandMaster *commandMaster, CommandFrame *command, uint32_t size);
    DEVICE_STATUS CommandMaster_SendCommandAsync(CommandMaster *commandMaster, CommandFrame *command, uint32_t size);

    DEVICE_STATUS CommandMaster_WaitForComplete(CommandMaster *commandMaster, ULONG timeout);

    void CommandMaster_DoTxComplete_(CommandMaster *commandMaster);

    void CommandMaster_DoRxComplete_(CommandMaster *commandMaster);

    void CommandMaster_DoError_(CommandMaster *commandMaster);

#ifdef __cplusplus
}
#endif

#endif // ___COMMAND_H__