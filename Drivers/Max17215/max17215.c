#include "max17215.h"

void MAX17215_Init(MAX17215 *dev, SWI *swi)
{
    dev->swi = swi;
}

uint16_t MAX17215_ReadRegister(MAX17215 *dev, uint8_t reg)
{
    uint8_t lsb, msb;

    // 1. reset SWI bus
    SWI_ResetPulse(dev->swi);

    // 2. send READ command (example opcode placeholder)
    SWI_WriteByte(dev->swi, 0xA1);

    // 3. register address
    SWI_WriteByte(dev->swi, reg);

    // 4. read data (LSB first)
    SWI_ReadBit(dev->swi, NULL, NULL); // placeholder bit sampling

    lsb = 0x00; // will be replaced by real capture logic

    SWI_ReadBit(dev->swi, NULL, NULL);
    msb = 0x00;

    return (uint16_t)(msb << 8) | lsb;
}



void MAX17215_WriteRegister(MAX17215 *dev, uint8_t reg, uint16_t value)
{
    SWI_ResetPulse(dev->swi);

    SWI_WriteByte(dev->swi, 0xA0); // WRITE opcode placeholder
    SWI_WriteByte(dev->swi, reg);

    SWI_WriteByte(dev->swi, (uint8_t)(value & 0xFF));
    SWI_WriteByte(dev->swi, (uint8_t)(value >> 8));
}


