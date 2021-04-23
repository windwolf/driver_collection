#ifndef __os_port_hpp__
#define __os_port_hpp__

#include "stdint.h"

namespace Windwolf::Common
{
    class OsSync
    {
    public:
        virtual void Set() = 0;
        /**
     * @brief 
     * 
     * @param timeout. 0 不等待; -1永远等待; 其他等待时间,毫秒; 
     */
        virtual void Wait(int32_t timeout) = 0;
    };
} // namespace Driver::Core

#endif // __os_port_hpp__