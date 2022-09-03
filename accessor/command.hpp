#ifndef __WWDEVICE_ACCESSOR_COMMAND_HPP__
#define __WWDEVICE_ACCESSOR_COMMAND_HPP__
#include "base.hpp"
#include "buffer.hpp"

#include "os.hpp"
#include "wait_handler.hpp"
namespace ww::accessor
{
using namespace ww::os;
enum CommandFrameMode
{
    CommandFrameMode_Skip = 0,
    CommandFrameMode_1line = 1,
    CommandFrameMode_2line = 2,
    CommandFrameMode_4line = 3,
};

enum CommandFramePhase
{
    CommandFramePhase_Command,
    CommandFramePhase_Address,
    CommandFramePhase_AltData,
    CommandFramePhase_DummyCycle,
    CommandFramePhase_Data,
};
struct CommandFrame
{
  public:
    ALIGN32 uint8_t commandId;
    uint32_t address;
    uint32_t altData;
    void *data;
    uint16_t dataSize;
    struct
    {
        CommandFrameMode commandMode : 2;
        // uint32_t commandBits : 2; // always 8bits

        CommandFrameMode addressMode : 2;
        DataWidth addressBits : 2;

        CommandFrameMode altDataMode : 2;
        DataWidth altDataBits : 2;

        CommandFrameMode dataMode : 2;
        DataWidth dataBits : 2;
        uint32_t isWrite : 1;

        uint32_t dummyCycles : 5;

        uint32_t isDdr : 1;
        uint32_t : 11;
    };
};

class Command
{
  public:
    Command(uint32_t timeout);
    Result send(CommandFrame &frame, WaitHandler &waitHandler);

  protected:
    virtual Result media_command_send(CommandFrame &frame, WaitHandler &waitHandler);

    virtual Result media_step_send(CommandFramePhase phase, void *data, uint32_t size,
                                   DataWidth dataWidth, bool isWrite, WaitHandler &waitHandler) = 0;
    virtual Result media_session_start(WaitHandler &waitHandler) = 0;
    virtual Result media_session_finish(WaitHandler &waitHandler) = 0;
    virtual Result media_send(CommandFrame &frame, WaitHandler &waitHandler);

  private:
    WaitHandler *_waitHandler;
    uint32_t _timeout;
    uint32_t _readyFlag;
    Result _session_begin(WaitHandler &waitHandler, uint32_t scope);
    Result _session_end(WaitHandler &waitHandler, uint32_t scope);

    Result _do_step_send(CommandFramePhase phase, void *data, uint32_t size, DataWidth dataWidth,
                         bool isWrite, WaitHandler &waitHandler, uint32_t scope);
};

} // namespace ww::accessor

#endif // __WWDEVICE_ACCESSOR_COMMAND_HPP__