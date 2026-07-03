#include "executor.h"
#include "executor_backend.h"
#include "critical_section.h"
#include "time_utils.h"

static ExecutorJob *g_job = 0;
static volatile bool g_busy = false;

void Executor_Init(void)
{
    g_job = 0;
    g_busy = false;

    Executor_Backend_Init();
}

uint32_t Executor_Now(void)
{
    return Executor_Backend_Now();
}

/* ---------------- scheduling ---------------- */

bool Executor_Post(ExecutorJob *job)
{
    if (!job || !job->callback)
        return false;

    EXECUTOR_ENTER_CRITICAL();

    if (g_busy)
    {
        EXECUTOR_EXIT_CRITICAL();
        return false;
    }

    g_busy = true;
    g_job = job;

    Executor_Backend_Arm(job->deadline_us);

    EXECUTOR_EXIT_CRITICAL();
    return true;
}

void Executor_Cancel(void)
{
    EXECUTOR_ENTER_CRITICAL();

    g_job = 0;
    g_busy = false;

    Executor_Backend_Disarm();

    EXECUTOR_EXIT_CRITICAL();
}

/* ---------------- ISR entry ---------------- */

void Executor_IRQHandler(void)
{
    Executor_Backend_ClearIRQ();
    Executor_OnCompare();
}

/* ---------------- execution ---------------- */

void Executor_OnCompare(void)
{
    ExecutorJob *job;

    EXECUTOR_ENTER_CRITICAL();

    if (!g_busy || !g_job)
    {
        EXECUTOR_EXIT_CRITICAL();
        return;
    }

    job = g_job;

    /* clear ownership before callback (re-entrancy safety) */
    g_job = 0;
    g_busy = false;

    EXECUTOR_EXIT_CRITICAL();

    job->callback(job);
}