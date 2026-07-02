#include "scheduler_tim12.h"
/*
 * Skeleton implementation.
 * TODO:
 * 1. Enable RCC clocks for TIM12 and GPIO.
 * 2. Configure TIM12 at 1 MHz.
 * 3. Enable update + CC1 interrupts.
 * 4. Extend 16-bit counter to 32-bit.
 * 5. Invoke registered callback on CC1 match.
 *
 * This file intentionally contains only the architectural skeleton.
 * The complete implementation will be developed module-by-module.
 */
static SchedulerBackend *g_backend = 0;
void SchedulerBackend_Init(SchedulerBackend *backend){g_backend=backend;}
void SchedulerBackend_RegisterCallback(SchedulerBackend *backend, SchedulerCallback cb, void *ctx){
    backend->callback=cb; backend->context=ctx;
}
void SchedulerTIM12_Init(SchedulerBackend *backend){SchedulerBackend_Init(backend);}
SchedulerTime SchedulerBackend_GetTime(void){return 0;}
void SchedulerBackend_Arm(SchedulerTime timestamp){(void)timestamp;}


void TIM8_BRK_TIM12_IRQHandler(void)
{
    if ((TIM12->SR & TIM_SR_CC1IF) != 0U)
    {
        TIM12->SR &= ~TIM_SR_CC1IF;

        Scheduler_ISR();
    }

    if ((TIM12->SR & TIM_SR_UIF) != 0U)
    {
        TIM12->SR &= ~TIM_SR_UIF;

        timerUpper += 0x10000;
    }
}