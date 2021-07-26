#ifndef ___SHARED_H__
#define ___SHARED_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#define BOOL uint8_t

#define EVENTS_CLEAR_FLAGS(eg) (tx_event_flags_set(&eg, 0, TX_AND))
#define EVENTS_SET_FLAGS(eg, flags) (tx_event_flags_set(&eg, flags, TX_OR))
#define EVENTS_RESET_FLAGS(eg, flags) (tx_event_flags_set(&eg, ~flags, TX_AND))

#define ALIGN32 __attribute__((aligned(32)))
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#define min(a, b) (((a) <= (b)) ? (a) : (b))
#define max(a, b) (((a) >= (b)) ? (a) : (b))

    uint32_t fast_log2(uint32_t _val);
//typedef uint8_t bool;
//#define true 1
//#define false 0

#define OP_RESULT uint32_t

#define OP_RESULT_OK 0x00000000
#define OP_RESULT_BUSY 0x80000000
#define OP_RESULT_NOT_SUPPORT 0x40000000
#define OP_RESULT_PARAMETER_ERROR 0x20000000
#define OP_RESULT_NO_MATCH 0x10000000
#define OP_RESULT_GENERAL_ERROR 0x01000000

    typedef void (*EventHandler)(void *sender, void *host, void *event);

#ifdef __cplusplus
}
#endif

#endif // ___SHARED_H__