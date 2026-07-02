#include "scheduler.h"

static void Scheduler_InsertSorted(Scheduler *s, SchedulerEvent e)
{
    if (s->count >= SCHEDULER_MAX_EVENTS)
        return;

    int i = s->count - 1;

    while (i >= 0 && s->events[i].time > e.time)
    {
        s->events[i + 1] = s->events[i];
        i--;
    }

    s->events[i + 1] = e;
    s->count++;
}


void Scheduler_Init(Scheduler *s, SchedulerBackend *backend)
{
    s->backend = backend;
    s->count = 0;
}


void Scheduler_Add(Scheduler *s, SchedulerEvent event)
{
    Scheduler_InsertSorted(s, event);

    // always arm next earliest event
    SchedulerBackend_Arm(s->events[0].time);
}


void Scheduler_Run(Scheduler *s)
{
    if (s->count == 0)
        return;

    SchedulerEvent e = s->events[0];

    // shift queue
    for (int i = 1; i < s->count; i++)
    {
        s->events[i - 1] = s->events[i];
    }

    s->count--;

    // execute handler
    e.handler(e.context);

    // re-arm next event
    if (s->count > 0)
    {
        SchedulerBackend_Arm(s->events[0].time);
    }
}


void Scheduler_OnTick(Scheduler *s)
{
    (void)s;
}