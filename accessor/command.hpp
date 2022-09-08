#ifndef __WWDEVICE_ACCESSOR_COMMAND_HPP__
#define __WWDEVICE_ACCESSOR_COMMAND_HPP__
#include "base.hpp"
#include "buffer.hpp"

#include "os.hpp"
#include "wait_handler.hpp"
namespace ww::accessor
{
using namespace ww::os;
enum class CommandFrameMode
{
    Skip = 0,
    Line1 = 1,
    Line2 = 2,
    Line4 = 3,
};

enum class CommandFramePhase
{
    Command,
    Address,
    AltData,
    DummyCycle,
    Data,
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
    virtual Result media_session_start() = 0;
    virtual Result media_session_finish() = 0;
    virtual Result media_command_send(CommandFrame &frame) = 0;

  protected:
    uint32_t _timeout;
    WaitHandler *_waitHandler;

  private:
    uint32_t _readyFlag;
};

} // namespace ww::accessor

#endif // __WWDEVICE_ACCESSOR_COMMAND_HPP__