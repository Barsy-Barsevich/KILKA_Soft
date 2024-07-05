/* 
 * File:   MPU6050_C_Barsotion.h
 * Author: victor
 *
 * Created on May 8, 2024, 1:17 AM
 */

#ifndef MPU6050_C_BARSOTION_H
#define	MPU6050_C_BARSOTION_H

#include "defines.h"

#include "MPU6050_RegMap.h"
#include "Interface.h"
#include "Time.h"
#include "Uartoma.h"

#define PI              3.1428

#define ACCEL_COEF_16G  16.0/32768.0//0.00048828125F
#define ACCEL_COEF_8G   8.0/32768.0//0.000244140625F
#define ACCEL_COEF_4G   4.0/32768.0//0.0001220703125F
#define ACCEL_COEF_2G   2.0/32768.0//0.00006103515625F
#define GYRO_COEF_2000  2000.0/32768.0//*3.14159265359/180.0// 2000/32768*PI/180
#define GYRO_COEF_1000  1000.0/32768.0//*3.14159265359/180.0// 1000/32768*PI/180
#define GYRO_COEF_500   500.0/32768.0//*3.14159265359/180.0 // 500/32768*PI/180
#define GYRO_COEF_250   250.0/32768.0//*3.14159265359/180.0 // 250/32768*PI/180

typedef enum __Bandwidth
{
    BANDWIDTH_260HZ = 0,
    BANDWIDTH_184HZ = 1,
    BANDWIDTH_94HZ = 2,
    BANDWIDTH_44HZ = 3,
    BANDWIDTH_21HZ = 4,
    BANDWIDTH_10HZ = 5,
    BANDWIDTH_5HZ = 6
} MPU6050_BandwidthTypeDef;

typedef enum __AccelScale
{
    ACCEL_SCALE_16G = 0b00011000,
    ACCEL_SCALE_8G = 0b00010000,
    ACCEL_SCALE_4G = 0b00001000,
    ACCEL_SCALE_2G = 0b00000000
} MPU6050_AccelScaleTypeDef;

typedef enum
{
    GYRO_SCALE_2000 = 0b00011000,
    GYRO_SCALE_1000 = 0b00010000,
    GYRO_SCALE_500 = 0b00001000,
    GYRO_SCALE_250 = 0b00000000            
} MPU6050_GyroScaleTypeDef;

typedef struct __XYZ
{
    float x;
    float y;
    float z;
} MPU6050_XYZ_TypeDef;

typedef struct __Filter
{
    float err_measure;
	float err_estimate;
	float q;
	float last_estimate;
} MPU6050_Filter_TypeDef;

typedef struct __MPU6050_Descriptor
{
    uint8_t address;
    //interface
    Interface_TypeDef* interface;
    //coefficients
    float accel_coef;
    float gyro_coef;
    //output data
    MPU6050_XYZ_TypeDef accel;
    MPU6050_XYZ_TypeDef gyro;
    //filter
    MPU6050_Filter_TypeDef filter_x;
    MPU6050_Filter_TypeDef filter_y;
    MPU6050_Filter_TypeDef filter_z;
    //bias
    MPU6050_XYZ_TypeDef bias;
    MPU6050_XYZ_TypeDef eps;
    //Integral & differencial
    float time_metka;
    MPU6050_XYZ_TypeDef pre_gyro;
    MPU6050_XYZ_TypeDef angle;
    MPU6050_XYZ_TypeDef angle_acceleration;
    MPU6050_XYZ_TypeDef acc_angle;
    float acc_total;
} MPU6050_TypeDef;


bool MPU6050_Init(MPU6050_TypeDef* local, Interface_TypeDef* i, uint8_t addr);

bool MPU6050_setBandwidth(MPU6050_TypeDef* local, MPU6050_BandwidthTypeDef bw);
bool MPU6050_setAccelScale(MPU6050_TypeDef* local, MPU6050_AccelScaleTypeDef scale);
bool MPU6050_setGyroScale(MPU6050_TypeDef* local, MPU6050_GyroScaleTypeDef scale);
void MPU6050_calculateAccel(MPU6050_TypeDef* local, uint8_t* accel);
void MPU6050_calculateGyro(MPU6050_TypeDef* local, uint8_t* gyro, bool filter);
//Filter
void MPU6050_setFilterParameters(MPU6050_Filter_TypeDef* channel, float mea_e, float est_e, float q);
float MPU6050_Filtered(MPU6050_Filter_TypeDef* channel, float value);
//Angles and angle accelerations
void MPU6050_calculateRPY(MPU6050_TypeDef* local, float dt);
void MPU6050_AngleCorrection(MPU6050_TypeDef* local);
//Calibration
void MPU6050_CalibrateGyro(MPU6050_TypeDef* local);
//Axes convercion (weak)
void MPU6050_AxesConversion(uint8_t* raw);

#endif	/* MPU6050_C_BARSOTION_H */

