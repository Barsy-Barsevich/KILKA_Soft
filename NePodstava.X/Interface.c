#include "Interface.h"

void Interface_Init(Interface_TypeDef* local, uint8_t addr)
{
    local->addr = addr;
}

bool readRegister(Interface_TypeDef* local, uint8_t reg, uint8_t* buf)
{
    return TWIreadRegister(local->addr, reg, buf);
}

bool writeRegister(Interface_TypeDef* local, uint8_t reg, uint8_t data)
{
    return TWIwriteRegister(local->addr, reg, data);
}