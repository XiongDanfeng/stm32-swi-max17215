#include "executor_backend.h"
#include <stdint.h>

static uint32_t fake_time = 0;

void Executor_Backend_Init(void)
{
    fake_time = 0;
}

uint32_t Executor_Backend_Now(void)
{
    return fake_time;
}

void Executor_Backend_Arm(uint32_t deadline_us)
{
    fake_time = deadline_us;
}

void Executor_Backend_Disarm(void)
{
}

void Executor_Backend_ClearIRQ(void)
{
}