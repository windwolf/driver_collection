#ifndef ___MINIUNIT_H__
#define ___MINIUNIT_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* file: minunit.h */
#define mu_assert(message, test) \
    do                           \
    {                            \
        if (!(test))             \
            return message;      \
    } while (0)

#define mu_run_test(test)       \
    do                          \
    {                           \
        char *message = test(); \
        tests_run++;            \
        if (message)            \
            return message;     \
    } while (0)
    extern int tests_run;

#define mu_assert_equals(message, a, b) \
    do                                  \
    {                                   \
        if (a != b)                     \
            return message;             \
    } while (0)

#define mu_assert_vec_equals(message, a, b, size) \
    do                                            \
    {                                             \
        int i = size;                             \
        if (a[i] != b[i])                         \
            return message;                       \
    } while (++i < size)

#ifdef __cplusplus
}
#endif

#endif // ___MINIUNIT_H__