//
// Created by windwolf on 2021/4/28.
//

#ifndef STARTUP_OS_PORT_H
#define STARTUP_OS_PORT_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

#define CONVERT_REFERENCE(var, type) (*((type*)&var))

//void __osport_event_set_default(void *host, uint32_t flags) {
//
//} ;
//
//uint32_t __osport_event_peek_default(void *host, uint32_t flags) {
//    return 0;
//} ;
//
//uint32_t __osport_event_get_default(void *host, uint32_t flags, uint32_t timeout) {
//    return 0;
//} ;
//
//
//void __osport_event_set(void *host, uint32_t flags) __attribute__ ((weak, alias("__osport_event_set_default")));
//
//uint32_t __osport_event_peek(void *host, uint32_t flags) __attribute__ ((weak, alias("__osport_event_peek_default")));
//
//uint32_t __osport_event_get(void *host, uint32_t flags, uint32_t timeout) __attribute__ ((weak, alias("__osport_event_get_default")));


//void __attribute__ ((weak)) __osport_event_set(void *host, uint32_t flags) {
//
//} ;
//
//uint32_t __attribute__ ((weak)) __osport_event_peek(void *host, uint32_t flags) {
//    return 0;
//} ;
//
//uint32_t __attribute__ ((weak)) __osport_event_get(void *host, uint32_t flags, uint32_t timeout) {
//    return 0;
//} ;

#ifdef __cplusplus
extern "C"
{
#endif

#include "common/os_port.h"

extern void
__os_thread_start(void *host, const char *name, void (*pFunction)(void *), uint8_t *stack, uint32_t stackSize, int i1, int i2, int i3, int b);

extern void __osport_event_set(void *host, uint32_t flags);

extern uint32_t __osport_event_peek(void *host, uint32_t flags);

extern uint32_t __osport_event_get(void *host, uint32_t flags, uint32_t timeout);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
}
#endif

#endif //STARTUP_OS_PORT_H
