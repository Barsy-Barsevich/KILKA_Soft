/* 
 * File:   System.h
 * Author: victor
 *
 * Created on May 9, 2024, 8:51 PM
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

#include "defines.h"
#include "Interface.h"
#include "MPU6050_C_Barsotion.h"
#include "BMP388_C_Barsotion.h"
#include "W25_Driver.h"
#include "Servo16_C_Barsotion.h"
#include "Uartoma.h"


//==============================================================================

//MPU and BMP descriptors
#if MPU
MPU6050_TypeDef mpu;
Interface_TypeDef mpu_interface;
#endif
#if BMP0
BMP388_TypeDef bmp0;
Interface_TypeDef bmp0_interface;
#endif
#if BMP1 
BMP388_TypeDef bmp1;
Interface_TypeDef bmp1_interface;
#endif

//TWI data struct
struct 
{
    uint8_t a[6];
    uint8_t t[2];
    uint8_t g[6];
    uint8_t p0[3];
    uint8_t t0[3];
    uint8_t p1[3];
    uint8_t t1[3];
} Raw1;

//Data cycle log
char cycle_log[128];
//
char arr[50];

/* BMP main results */
float BMP_PresDiff; //absolute difference between BMP0 & BMP1 data
float Pressure;     //static pressure
float DynPressure;  //dynamic pressure
float Altitude;     //system altitude
float Temperature;  //system temperature
float VSpeed;       //vertical speed
float DifVSpeed;    //diff. altitude
float PreAltitude;  //var to contain previous altitude

/* System time */
uint32_t pre_micros;
float deltat;

//==============================================================================
//Mode flags
//Cycle counter
volatile uint32_t cycle_counter;
//TWI cycle counter
volatile uint8_t twi_mode;
volatile bool twi_error;
//Idle mode flag
volatile static bool active_mode;
bool idle_mode;
//Condition mode counter
uint8_t condition_mode;
//Detector counters
uint8_t apogee_detector_counter;
uint8_t landing_detector_counter;

//==============================================================================


//Only for LGT8F328P, clock config and UART TX pin config
void LGT8F328P_ClockConfig();
void UART_TX6();
//Beeps
void System_Bip(uint8_t number);
void System_BipError();
//MPU & BMP init and data preparing
void MPU_BMP_Init();
void MPU_BMP_Read();
void System_AltitudeVSpeedCalculations();
//Read cycle log, addr - number of cycle, send to UART
bool System_ReadCycleLog(uint32_t addr);
//Servo print signal only for idle mode
void System_ServoModify();
//Read command from UART, only in idle-mode
void System_ReadCommand();
//Log data
void System_LogData();
//
void System_VerifyConditions();
//
void System_SAS();

//#ifdef	__cplusplus
//}
//#endif

#endif	/* SYSTEM_H */

