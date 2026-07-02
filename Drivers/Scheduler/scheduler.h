#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "scheduler_types.h"
#include "scheduler_backend.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCHEDULER_MAX_EVENTS 16

typedef struct
{
    SchedulerBackend *backend;

    SchedulerEvent events[SCHEDULER_MAX_EVENTS];

    uint8_t count;

} Scheduler;

void Scheduler_Init(Scheduler *s, SchedulerBackend *backend);

void Scheduler_Add(Scheduler *s, SchedulerEvent event);

void Scheduler_Run(Scheduler *s);

void Scheduler_OnTick(Scheduler *s);

#ifdef __cplusplus
}
#endif

#endif