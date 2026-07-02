#ifndef SWI_CAPTURE_H
#define SWI_CAPTURE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    volatile uint8_t bit_buffer;
    volatile uint8_t bit_index;
    volatile uint8_t byte_ready;
} SWI_Capture;

void SWI_Capture_Init(SWI_Capture *c);

void SWI_Capture_Reset(SWI_Capture *c);

void SWI_Capture_SampleBit(SWI_Capture *c, uint8_t bit);

bool SWI_Capture_IsByteReady(SWI_Capture *c);

uint8_t SWI_Capture_ReadByte(SWI_Capture *c);

#ifdef __cplusplus
}
#endif

#endif