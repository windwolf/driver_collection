#ifndef ___LOG_H__
#define ___LOG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef LOG_MODULE
#ifndef LOG_MODULE__
#define LOG_MODULE__ LOG_MODULE
#endif
#else
#ifndef LOG_MODULE__
#define LOG_MODULE__ "LOG"
#endif
#endif

#ifdef LOG_LEVEL
#ifndef LOG_LEVEL__
#define LOG_LEVEL__ LOG_LEVEL
#endif
#else
#ifndef LOG_LEVEL__
#define LOG_LEVEL__ LOG_LEVEL_DEBUG
#endif
#endif

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4

#ifdef DEBUG
#define __LOG__(level, module, fmt, ...) \
    do                                   \
    {                                    \
        printf(level " " module ": ");   \
        printf(fmt, ##__VA_ARGS__);      \
        printf("\n");                    \
    } while (0)

#if (LOG_LEVEL__ >= LOG_LEVEL_ERROR)
#define LOG_E(fmt, ...) __LOG__("E", LOG_MODULE__, fmt, ##__VA_ARGS__)
#else
#define LOG_E(fmt, ...)
#endif

#if (LOG_LEVEL__ >= LOG_LEVEL_WARNING)
#define LOG_W(fmt, ...) __LOG__("W", LOG_MODULE__, fmt, ##__VA_ARGS__)
#else
#define LOG_W(fmt, ...)
#endif

#if (LOG_LEVEL__ >= LOG_LEVEL_DEBUG)
#define LOG_D(fmt, ...) __LOG__("D", LOG_MODULE__, fmt, ##__VA_ARGS__)
#else
#define LOG_D(fmt, ...)
#endif

#if (LOG_LEVEL__ >= LOG_LEVEL_INFO)
#define LOG_I(fmt, ...) __LOG__("I", LOG_MODULE__, fmt, ##__VA_ARGS__)
#else
#define LOG_I(fmt, ...)
#endif
#else
#define LOG_E(fmt, ...)
#define LOG_W(fmt, ...)
#define LOG_D(fmt, ...)
#define LOG_I(fmt, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif // ___LOG_H__