#ifndef ___MINIUNIT_H__
#define ___MINIUNIT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"

/* file: minunit.h */
#define MU_ASSERT(message, test)    \
    do                              \
    {                               \
        if (!(test))                \
            printf(message "\r\n"); \
    } while (0)

#define MU_RUN(test)                \
    do                              \
    {                               \
        char *message = test();     \
        if (message)                \
            printf(message "\r\n"); \
    } while (0)

#define MU_ASSERT_EQUALS(message, a, b) \
    do                                  \
    {                                   \
        if (a != b)                     \
            printf(message "\r\n");     \
    } while (0)

#define MU_ASSERT_VEC_EQUALS(message, a, b, size) \
    do                                            \
    {                                             \
        for (uint32_t i = 0; i < size; i++)       \
        {                                         \
            if (a[i] != b[i])                     \
                printf(message "\r\n");           \
        }                                         \
    } while (0)

#define MU_VEC_CLEAR(vec, size)             \
    do                                      \
    {                                       \
        for (uint32_t i = 0; i < size; i++) \
        {                                   \
            vec[i] = 0;                     \
        }                                   \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif // ___MINIUNIT_H__