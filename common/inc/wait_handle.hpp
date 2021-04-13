#ifndef __wait_handle_hpp__
#define __wait_handle_hpp__

#include "stdint.h"
#include "common.hpp"
#include "os_port.hpp"

namespace Windwolf::Common
{
    class WaitHandle
    {
    public:
        enum WAIT_STATUS
        {
            INIT,
            WAITING,
            DONE,
            CANCELED,
        };

        WaitHandle(OsSync *sync);

        void Start();
        void Wait();
        void Finish();
        void Cancel();
        void *GetPayload();
        void SetPayload(void *payload);
        WAIT_STATUS GetStatus();

    private:
        void *_payload;
        WAIT_STATUS _status;
        OsSync *_sync;
    };
} //Driver::Core

#endif // __wait_handle_hpp__