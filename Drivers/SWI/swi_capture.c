#include "swi_capture.h"

void SWI_Capture_Init(SWI_Capture *c)
{
    c->bit_buffer = 0;
    c->bit_index = 0;
    c->byte_ready = 0;
}

void SWI_Capture_Reset(SWI_Capture *c)
{
    c->bit_buffer = 0;
    c->bit_index = 0;
    c->byte_ready = 0;
}


void SWI_Capture_SampleBit(SWI_Capture *c, uint8_t bit)
{
    c->bit_buffer |= (bit & 0x1) << c->bit_index;
    c->bit_index++;

    if (c->bit_index >= 8)
    {
        c->byte_ready = 1;
        c->bit_index = 0;
    }
}


bool SWI_Capture_IsByteReady(SWI_Capture *c)
{
    return c->byte_ready;
}

uint8_t SWI_Capture_ReadByte(SWI_Capture *c)
{
    c->byte_ready = 0;
    return c->bit_buffer;
}