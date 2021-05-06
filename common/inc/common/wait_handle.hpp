//
// Created by windwolf on 2021/4/28.
//

#ifndef STARTUP_WAIT_HANDLE_HPP
#define STARTUP_WAIT_HANDLE_HPP

#include "os/os.hpp"
#include "common/shared.hpp"

namespace windwolf::common {
    using namespace windwolf::os;

    /**
     *      reset =[wait]
     *      reset =[finish]=> set
     *      reset =[cancel]=> canceled
     *      reset =[error]=> error
     */
    template<typename T>
    class WaitHandle {

    private:
        union {
            uint8_t value;
            struct {
                uint8_t canceled: 1;
                uint8_t error: 1;
            };
        } _flags;
        T _payload;
        OsEvent &_sync;
    public:

        explicit WaitHandle(OsEvent &sync);

        bool wait(bool reset);

        void setDone(T payload);

        void setCancel();

        void setError();

        T getPayload();
    };

    template
    class WaitHandle<void *>;

    template
    class WaitHandle<Buffer2<uint8_t >>;
}


#endif //STARTUP_WAIT_HANDLE_HPP
