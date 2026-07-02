#ifndef SWI_H
#define SWI_H

#include <stdint.h>
#include "scheduler_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SWI SWI;

typedef enum
{
    SWI_ACT_DRIVE_LOW,
    SWI_ACT_RELEASE,
    SWI_ACT_SAMPLE,
} SWI_ActionType;

typedef struct
{
    SchedulerTime time;
    SWI_ActionType action;
    void *context;
} SWI_ActionEvent;

typedef struct
{
    Scheduler *scheduler;
    SchedulerTime cursor;
} SWI;

void SWI_Init(SWI *swi, Scheduler *scheduler, SchedulerTime start_time);

void SWI_ResetPulse(SWI *swi);

void SWI_WriteBit(SWI *swi, uint8_t bit);

void SWI_WriteByte(SWI *swi, uint8_t byte);

void SWI_ReadBit(SWI *swi, void (*cb)(uint8_t bit, void *ctx), void *ctx);

#ifdef __cplusplus
}
#endif

#endif