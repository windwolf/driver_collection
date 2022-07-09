#ifndef ___BSP_SHARED_H__
#define ___BSP_SHARED_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "stdbool.h"

#define DEFINE_DEVICE_REGISTER_BEGIN(deviceName, itemCount)                 \
    static void *_##deviceName##_REGISTER_INSTENCE_[(uint32_t)(itemCount)]; \
    static inline uint32_t device_instance_index_get(void *bspBase)         \
    {                                                                       \
        uint32_t ins = (uint32_t)bspBase;                                   \
                                                                            \
        switch (ins)                                                        \
        {

#define DEFINE_DEVICE_REGISTER_ITEM(caseBspName, index) \
    case caseBspName:                                   \
        return index;

#define DEFINE_DEVICE_REGISTER_END(deviceName)                                   \
    default:                                                                     \
        return -1;                                                               \
        }                                                                        \
        }                                                                        \
        static inline void device_instance_register(void *device, void *bspBase) \
        {                                                                        \
            uint32_t index = device_instance_index_get(bspBase);                 \
            _##deviceName##_REGISTER_INSTENCE_[index] = device;                  \
        }                                                                        \
                                                                                 \
        static inline void *device_instance_find(void *bspBase)                  \
        {                                                                        \
            uint32_t index = device_instance_index_get(bspBase);                 \
            return _##deviceName##_REGISTER_INSTENCE_[index];                    \
        }

#define DEVICE_INSTANCE_REGISTER(device, bspBase) device_instance_register(device, bspBase)
#define DEVICE_INSTANCE_FIND(bspBase) device_instance_find(bspBase)

    void bsp_delay_ms(uint32_t ms); // delay ms
    void bsp_delay_us(uint32_t us); // delay us

    uint32_t bsp_get_tick_count(void);

#ifdef __cplusplus
}
#endif

#endif // ___BSP_SHARED_H__