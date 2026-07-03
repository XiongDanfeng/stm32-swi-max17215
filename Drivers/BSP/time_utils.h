#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline uint32_t Time_Next(uint32_t now, uint32_t delta_us)
{
    return now + delta_us;
}

/* wrap-safe comparison */
static inline bool Time_IsBefore(uint32_t a, uint32_t b)
{
    return (int32_t)(a - b) < 0;
}

static inline bool Time_IsAfter(uint32_t a, uint32_t b)
{
    return (int32_t)(b - a) < 0;
}

static inline uint32_t Time_Diff(uint32_t a, uint32_t b)
{
    return (uint32_t)(a - b);
}

#ifdef __cplusplus
}
#endif

#endif