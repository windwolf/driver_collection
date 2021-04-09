#ifndef __stream_h__
#define __stream_h__

namespace drivers
{

    class Stream
    {
    public:
        virtual void read() = 0;
        virtual void write() = 0;
    };

} // namespace drivers

#endif // __stream_h__