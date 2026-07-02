

#include "swi.h"
#include "scheduler.h"

#define T_RESET_LOW     480
#define T_RESET_HIGH    70
#define T_SLOT          60
#define T_RECOVERY      1
#define T_SAMPLE        15


volatile uint8_t g_sampling_enabled;

void SWI_Init(SWI *swi, Scheduler *scheduler, SchedulerTime start_time)
{
    swi->scheduler = scheduler;
    swi->cursor = start_time;
}

void SWI_ResetPulse(SWI *swi)
{
    SWI_Schedule(swi, 0, act_drive_low, NULL);
    SWI_Schedule(swi, T_RESET_LOW, act_release, NULL);
    SWI_Schedule(swi, T_RESET_HIGH, act_sample, NULL);
}

static void SWI_Schedule(SWI *swi,
                         SchedulerTime dt,
                         SchedulerEventHandler handler,
                         void *ctx)
{
    swi->cursor += dt;

    SchedulerEvent e;
    e.time = swi->cursor;
    e.handler = handler;
    e.context = ctx;

    Scheduler_Add(swi->scheduler, e);
}


void SWI_WriteBit(SWI *swi, uint8_t bit)
{
    if (bit)
    {
        // logic '1'
        SWI_Schedule(swi, 0, act_drive_low, NULL);
        SWI_Schedule(swi, 6, act_release, NULL);
        SWI_Schedule(swi, 54, act_sample, NULL);
    }
    else
    {
        // logic '0'
        SWI_Schedule(swi, 0, act_drive_low, NULL);
        SWI_Schedule(swi, 60, act_release, NULL);
    }
}

void SWI_WriteByte(SWI *swi, uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        SWI_WriteBit(swi, (byte >> i) & 1);
    }
}


void SWI_ReadBit(SWI *swi,
                 SWI_Capture *cap)
{
    // schedule read slot
    SWI_Schedule(swi, 0, act_drive_low, NULL);
    SWI_Schedule(swi, 6, act_release, NULL);

    // enable sampling window
    g_sampling_enabled = 1;

    SWI_Schedule(swi, 9, act_disable_sampling, NULL);
}


typedef struct
{
    void (*cb)(uint8_t bit, void *ctx);
    void *ctx;
} SWI_ReadCtx;


static void act_drive_low(void *ctx) { (void)ctx; }
static void act_release(void *ctx)   { (void)ctx; }
static void act_sample(void *ctx)    { (void)ctx; }


static void act_capture(void *ctx)
{
    SWI_ReadCtx *r = (SWI_ReadCtx *)ctx;
    r->cb(1, r->ctx); // placeholder
}

static void act_disable_sampling(void *ctx)
{
    (void)ctx;
    g_sampling_enabled = 0;
}


