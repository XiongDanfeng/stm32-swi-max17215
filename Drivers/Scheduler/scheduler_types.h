#ifndef SCHEDULER_TYPES_H
#define SCHEDULER_TYPES_H

#include <stdint.h>

typedef uint32_t SchedulerTime;

typedef struct SchedulerEvent SchedulerEvent;

typedef void (*SchedulerEventHandler)(void *context);

struct SchedulerEvent
{
    SchedulerTime time;
    SchedulerEventHandler handler;
    void *context;
};

#endif