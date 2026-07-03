#ifndef EXECUTOR_BACKEND_H
#define EXECUTOR_BACKEND_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void Executor_Backend_Init(void);

uint32_t Executor_Backend_Now(void);

/* backend owns compare + interrupt logic */
void Executor_Backend_Arm(uint32_t deadline_us);

void Executor_Backend_Disarm(void);

void Executor_Backend_ClearIRQ(void);

#ifdef __cplusplus
}
#endif

#endif