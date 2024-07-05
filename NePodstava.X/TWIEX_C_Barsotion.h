/* 
 * File:   TWIEX_C_Barsotion.h
 * Author: victor
 *
 * Created on May 8, 2024, 7:09 PM
 */

#ifndef TWIEX_C_BARSOTION_H
#define	TWIEX_C_BARSOTION_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

#include "defines.h"

#define TWI_ON_INTERRUPTS
#define TWI_TWBR()  ((F_CPU / 400000UL - 16) >> 1)

//typedef struct
//{
//    uint8_t addr;
//    volatile uint8_t mode;
//} TWI_TypeDef;


void TWIbegin();

#ifdef TWI_ON_INTERRUPTS
//inline void TWIdisable_twi_timer();
//inline void TWIenable_twi_timer();
#endif
bool TWIwriteRegister(uint8_t addr, uint8_t reg, uint8_t data);
bool TWIreadRegister(uint8_t addr, uint8_t reg, uint8_t* out);
bool TWI_start();
bool TWI_write(uint8_t data);
bool TWI_read(bool ACKM, uint8_t* data);
bool TWI_stop();
#ifdef TWI_ON_INTERRUPTS
void TWIstartQueue(uint8_t* mode);
void TWIstart();
void TWIwrite(uint8_t data);
void TWIrequest(bool ACKM);
uint8_t TWIread();
void TWIstop();
void TWIsetErrorTime(uint8_t mks);
#endif
void TWIend();

//#ifdef	__cplusplus
//}
//#endif

#endif	/* TWIEX_C_BARSOTION_H */

