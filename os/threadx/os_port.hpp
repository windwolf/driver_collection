//
// Created by windwolf on 2021/4/26.
//

#ifndef STARTUP_OS_PORT_HPP
#define STARTUP_OS_PORT_HPP

#include "common/os.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

#include "tx_api.h"

#ifdef __cplusplus
}
#endif


namespace Windwolf::Drivers::OsPort {
    class ThreadxOsSync : public OsSync {
    public:
        ThreadxOsSync(TX_EVENT_FLAGS_GROUP *group, ULONG flags)
                : _group(group), _flags(flags) {
        }

        void Set() final {
            tx_event_flags_set(_group, _flags, TX_AND);
        }

        void Wait(int32_t timeout) final {
            ULONG actualFlag;
            if (timeout < 0) {
                tx_event_flags_get(_group, _flags, TX_AND_CLEAR, &actualFlag, TX_WAIT_FOREVER);
            } else if (timeout == 0) {
                tx_event_flags_get(_group, _flags, TX_AND_CLEAR, &actualFlag, TX_NO_WAIT);
            } else {
                tx_event_flags_get(_group, _flags, TX_AND_CLEAR, &actualFlag, timeout);
            }
        }

    private:
        TX_EVENT_FLAGS_GROUP *_group;
        ULONG _flags;
    };
}


#endif //STARTUP_OS_PORT_HPP
