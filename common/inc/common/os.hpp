#ifndef __os_hpp__
#define __os_hpp__

#include "stdint.h"

namespace Windwolf::Common {
    class OsEvent {
    public:
        virtual void Set() = 0;

        virtual bool Peek() = 0;

        /**
     * @brief
     *
     * @param timeout. 0 不等待; -1永远等待; 其他等待时间,毫秒;
     */
        virtual bool Get(int32_t timeout) = 0;
    };


}; // namespace Driver::Core

#endif // __os_hpp__
