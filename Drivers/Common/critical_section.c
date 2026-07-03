#include "critical_section.h"
#include "stm32f4xx.h"

static uint32_t primask;

void CriticalSection_Enter(void)
{
    primask = __get_PRIMASK();
    __disable_irq();
}

void CriticalSection_Exit(void)
{
    if (!primask)
    {
        __enable_irq();
    }
}