/* 
 * File:   Servo16_C_Barsotion.h
 * Author: victor
 *
 * Created on June 27, 2024, 2:36 AM
 */

#ifndef SERVO16_C_BARSOTION_H
#define	SERVO16_C_BARSOTION_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

#include "defines.h"

//Number of ticks per 3mks
#define SERVO_TOP       (uint16_t)((F_CPU>>3) / 333/*Hz*/)
#define __SERVO_ANGLE_TO_MKS(angle)     (500. + (2000./180) * angle)
//4 ticks per mks
#define __SERVO_MKS_TO_OCR(mks)         ((float)((F_CPU>>3)/1000000UL) * mks)

typedef struct
{
    float angle;
    int16_t bias;
} Servo_TypeDef;

Servo_TypeDef servoa, servob, servoc, servod;

//Servo system init
void Servo16_Init();
//Print all 4 servo
void Servo16_Print();

//#ifdef	__cplusplus
//}
//#endif

#endif	/* SERVO16_C_BARSOTION_H */

