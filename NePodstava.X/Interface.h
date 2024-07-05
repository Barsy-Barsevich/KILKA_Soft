/* 
 * File:   Interface.h
 * Author: victor
 *
 * Created on May 8, 2024, 11:13 PM
 */

#ifndef INTERFACE_H
#define	INTERFACE_H

#include "defines.h"
#include "TWIEX_C_Barsotion.h"

typedef struct
{
    uint8_t addr;
} Interface_TypeDef;

void Interface_Init(Interface_TypeDef* local, uint8_t addr);
bool readRegister(Interface_TypeDef* local, uint8_t reg, uint8_t* buf);
bool writeRegister(Interface_TypeDef* local, uint8_t reg, uint8_t data);

#endif	/* INTERFACE_H */

