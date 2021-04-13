#ifndef __stream_h__
#define __stream_h__

namespace Windwolf::Common
{
    class StreamDevice
    {
    public:
        virtual void read() = 0;
        virtual void write() = 0;
    };

} // namespace Windwolf::Common

#endif // __stream_h__