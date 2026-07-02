#ifndef MAX17215_H
#define MAX17215_H

#include <stdint.h>
#include "swi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    SWI *swi;
} MAX17215;

void MAX17215_Init(MAX17215 *dev, SWI *swi);

uint16_t MAX17215_ReadRegister(MAX17215 *dev, uint8_t reg);

void MAX17215_WriteRegister(MAX17215 *dev, uint8_t reg, uint16_t value);

#ifdef __cplusplus
}
#endif

#endif