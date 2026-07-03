#include "executor_backend.h"
#include "executor.h"
#include "stm32f4xx.h"

static void TIM12_Config(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;

    /* 1 MHz tick = 1us */
    TIM12->PSC = (SystemCoreClock / 1000000U) - 1U;
    TIM12->ARR = 0xFFFFFFFF;

    TIM12->CNT = 0;

    /* enable compare interrupt */
    TIM12->DIER |= TIM_DIER_CC1IE;

    TIM12->CR1 |= TIM_CR1_CEN;

    NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 5);
    NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
}

void Executor_Backend_Init(void)
{
    TIM12_Config();
}

uint32_t Executor_Backend_Now(void)
{
    return TIM12->CNT;
}

void Executor_Backend_Arm(uint32_t deadline_us)
{
    TIM12->CCR1 = deadline_us;

    /* ensure interrupt enabled */
    TIM12->DIER |= TIM_DIER_CC1IE;
}

void Executor_Backend_Disarm(void)
{
    TIM12->DIER &= ~TIM_DIER_CC1IE;
}

void Executor_Backend_ClearIRQ(void)
{
    TIM12->SR &= ~TIM_SR_CC1IF;
}

/* ISR */
void TIM8_BRK_TIM12_IRQHandler(void)
{
    Executor_IRQHandler();
}