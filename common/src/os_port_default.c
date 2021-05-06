//
// Created by windwolf on 2021/4/28.
//
#include "stdint.h"


void __attribute__ ((weak)) __osport_event_set(void *host, uint32_t flags) {

};

uint32_t __attribute__ ((weak)) __osport_event_peek(void *host, uint32_t flags) {
    return 0;
};

uint32_t __attribute__ ((weak)) __osport_event_get(void *host, uint32_t flags, uint32_t timeout) {
    return 0;
};
