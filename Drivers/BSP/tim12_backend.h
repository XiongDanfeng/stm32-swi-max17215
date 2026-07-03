#ifndef TIM12_BACKEND_H
#define TIM12_BACKEND_H
#include <stdint.h>
void TIM12_Backend_Init(void); uint32_t TIM12_Backend_Now(void); void TIM12_Backend_SetCompare(uint32_t); void TIM12_Backend_EnableInterrupt(void); void TIM12_Backend_DisableInterrupt(void); void TIM12_Backend_ClearInterruptFlag(void);
#endif