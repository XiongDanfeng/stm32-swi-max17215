#ifndef SCHEDULER_BACKEND_H
#define SCHEDULER_BACKEND_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef uint32_t SchedulerTime;
typedef void (*SchedulerCallback)(void *context);
typedef struct {
    SchedulerCallback callback;
    void *context;
} SchedulerBackend;
void SchedulerBackend_Init(SchedulerBackend *backend);
void SchedulerBackend_RegisterCallback(SchedulerBackend *backend, SchedulerCallback cb, void *ctx);
SchedulerTime SchedulerBackend_GetTime(void);
void SchedulerBackend_Arm(SchedulerTime timestamp);
#ifdef __cplusplus
}
#endif
#endif
