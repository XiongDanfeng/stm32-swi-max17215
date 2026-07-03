#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ExecutorJob ExecutorJob;

typedef void (*ExecutorCallback)(ExecutorJob *job);

struct ExecutorJob
{
    uint32_t deadline_us;
    ExecutorCallback callback;
    void *context;
};

void Executor_Init(void);
uint32_t Executor_Now(void);

/* single active transaction rule (I1/I2) */
bool Executor_Post(ExecutorJob *job);
void Executor_Cancel(void);

void Executor_IRQHandler(void);

/* backend calls this */
void Executor_OnCompare(void);

#ifdef __cplusplus
}
#endif

#endif