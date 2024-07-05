/* 
 * File:   BMP388_C_Barsotion.h
 * Author: victor
 *
 * Created on May 9, 2024, 4:08 PM
 */

#ifndef BMP388_C_BARSOTION_H
#define	BMP388_C_BARSOTION_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

#include "defines.h"

#include "TWIEX_C_Barsotion.h"
#include "BMP388_RegMap.h"
#include "Interface.h"
#include "Uartoma.h"


#define _SEA_LEVEL                   101325.0


typedef enum __BMP388_Oversampling
{
    OVERSAMPLING_1X = 0b000,
    OVERSAMPLING_2X = 0b001,
    OVERSAMPLING_4X = 0b010,
    OVERSAMPLING_8X = 0b011,
    OVERSAMPLING_16X = 0b100,
    OVERSAMPLING_32X = 0b101
} BMP388_Oversampling_TypeDef;

typedef enum __BMP388_IIR_Filter
{
    IIR_FILTER_COEF_0 = 0b000,
    IIR_FILTER_COEF_1 = 0b001,
    IIR_FILTER_COEF_3 = 0b010,
    IIR_FILTER_COEF_7 = 0b011,
    IIR_FILTER_COEF_15 = 0b100,
    IIR_FILTER_COEF_31 = 0b101,
    IIR_FILTER_COEF_63 = 0b110,
    IIR_FILTER_COEF_127 = 0b111
} BMP388_IIR_Filter_TypeDef;

typedef enum __BMP388_ODR
{
    ODR_200_HZ =    0x00,
    ODR_100_HZ =    0x01,
    ODR_50_HZ =     0x02,
    ODR_25_HZ =     0x03,
    ODR_12p5_HZ =   0x04,
    ODR_6p25_HZ =   0x05,
    ODR_3p1_HZ =    0x06,
    ODR_1p5_HZ =    0x07,
    ODR_0p78_HZ =   0x08,
    ODR_0p39_HZ =   0x09,
    ODR_0p2_HZ =    0x0A,
    ODR_0p1_HZ =    0x0B,
    ODR_0p05_HZ =   0x0C,
    ODR_0p02_HZ =   0x0D,
    ODR_0p01_HZ =   0x0E,
    ODR_0p006_HZ =  0x0F,
    ODR_0p003_HZ =  0x10,
    ODR_0p0015_HZ = 0x11
} BMP388_ODR_TypeDef;

#define TEMP_FLAG                   0x01
#define PRESS_FLAG                  0x02

typedef struct
{
    uint8_t address;
    //interface
    Interface_TypeDef* interface;
    struct __Factory_Parameters
    {
        float T1, T2, T3, P1, P2, P3, P4;
        float P5, P6, P7, P8, P9, P10, P11;
    } par;
    //output data
    float temperature, pressure, altitude;
    //osr reg (for setting) 
    uint8_t osr_reg;
    //static pressure
    float static_pressure, static_altitude;
} BMP388_TypeDef;

//Init
bool BMP388_begin(BMP388_TypeDef* local, Interface_TypeDef* i, uint8_t addr);
//Settings
void BMP388_setTempOversampling(BMP388_TypeDef* local, uint8_t value);
void BMP388_setPresOversampling(BMP388_TypeDef* local, uint8_t value);
void BMP388_setIIRFilterCoeff(BMP388_TypeDef* local, uint8_t value);
void BMP388_setOutputDataRate(BMP388_TypeDef* local, uint8_t value);
//Calibration
void BMP388_readCalibrationData(BMP388_TypeDef* local);      
float BMP388_AdvancedShift(float f, uint8_t value);
void BMP388_readStaticPressure(BMP388_TypeDef* local);
//Read
void BMP388_calculateTemperature(BMP388_TypeDef* local, uint8_t* raw);
void BMP388_calculatePressure(BMP388_TypeDef* local, uint8_t* raw);
void BMP388_calculateAltitude(BMP388_TypeDef* local);


//#ifdef	__cplusplus
//}
//#endif

#endif	/* BMP388_C_BARSOTION_H */