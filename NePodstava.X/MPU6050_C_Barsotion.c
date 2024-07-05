#include "MPU6050_C_Barsotion.h"

bool MPU6050_Init(MPU6050_TypeDef* local, Interface_TypeDef* i, uint8_t addr)
{
    local->address = addr;
    local->interface = i;
    Interface_Init(local->interface, local->address);
    //bias
    local->bias.x = 0;
    local->bias.y = 0;
    local->bias.z = 0;
    //angled
    local->angle.x = 0;
    local->angle.y = 0;
    local->angle.z = 0;
    //Start reset
    if (writeRegister(local->interface, MPU6050_PWR_MGMT_1,
        (1<<MPU6050_DEVICE_RESET)|  //?? ?????, ??? ?? ??????? ?? ????????
        (0<<MPU6050_SLEEP)|         //?? ????
        (0<<MPU6050_CYCLE)|         //cycle mode ??? ?? ?????
        (1<<MPU6050_TEMP_DIS)|      //????????????? ?????? ??? ?? ?????
        (0<<MPU6050_CLKSEL2)|       //internal 8mhz oscillator
        (0<<MPU6050_CLKSEL1)|
        (0<<MPU6050_CLKSEL0)) == false) return false;
    _delay_ms(100);
    if (writeRegister(local->interface, MPU6050_SIGNAL_PATH_RESET, 0b00000111)==false)
    {
        return false;
    }
    _delay_ms(100);
    if (writeRegister(local->interface, MPU6050_PWR_MGMT_1, 0b00001000) == false)
    {
        return false;
    }
    //Test MPU6050 ID
    uint8_t data;
    readRegister(local->interface, MPU6050_WHO_AM_I, &data);
    if (data != 0x68) return false;
    // Set BANDWITH
    if (MPU6050_setBandwidth(local, BANDWIDTH_260HZ) == false) return false;
    //Gyro scale 250dps, accel scale 2g
    if (MPU6050_setAccelScale(local, ACCEL_SCALE_2G) == false) return false;
    if (MPU6050_setGyroScale(local, GYRO_SCALE_250) == false) return false;
    //Switch on all the axes
    if (writeRegister(local->interface, MPU6050_PWR_MGMT_2,
        (0<<MPU6050_STBY_XA)|
        (0<<MPU6050_STBY_YA)|
        (0<<MPU6050_STBY_ZA)|
        (0<<MPU6050_STBY_XG)|
        (0<<MPU6050_STBY_YG)|
        (0<<MPU6050_STBY_ZG)) == false) return false;
    /*// ????? ?????! ???????? bypass mode ???? twi, ??? ????? ??????????? ??
    //????? ????????
    if (writeRegister(MPU_ADDR, MPU_INT_PIN_CFG, 0b00000010) == false) {
        return false;
    }*/
    return true;
}



bool MPU6050_setBandwidth(MPU6050_TypeDef* local, MPU6050_BandwidthTypeDef bw)
{
    return writeRegister(local->interface, MPU6050_CONFIG, bw);
}

bool MPU6050_setAccelScale(MPU6050_TypeDef* local, MPU6050_AccelScaleTypeDef scale)
{
    switch (scale)
    {
        case ACCEL_SCALE_16G: local->accel_coef = ACCEL_COEF_16G; break;
        case ACCEL_SCALE_8G: local->accel_coef = ACCEL_COEF_8G; break;
        case ACCEL_SCALE_4G: local->accel_coef = ACCEL_COEF_4G; break;
        case ACCEL_SCALE_2G: local->accel_coef = ACCEL_COEF_2G; break;
        default: local->accel_coef = ACCEL_COEF_16G; break;
    }
    return writeRegister(local->interface, MPU6050_ACCEL_CONFIG, scale);
}

bool MPU6050_setGyroScale(MPU6050_TypeDef* local, MPU6050_GyroScaleTypeDef scale)
{
    switch (scale)
    {
        case GYRO_SCALE_2000: local->gyro_coef = GYRO_COEF_2000; break;
        case GYRO_SCALE_1000: local->gyro_coef = GYRO_COEF_1000; break;
        case GYRO_SCALE_500: local->gyro_coef = GYRO_COEF_500; break;
        case GYRO_SCALE_250: local->gyro_coef = GYRO_COEF_250; break;
        default: local->gyro_coef = GYRO_COEF_2000; break;
    }
    return writeRegister(local->interface, MPU6050_GYRO_CONFIG, scale);
}

//==============================================================================
void MPU6050_calculateAccel(MPU6050_TypeDef* local, uint8_t* accel)
{
    MPU6050_AxesConversion(accel);
    int16_t raw; 
    raw = ((int16_t)accel[0]<<8 | (int16_t)accel[1]);
    local->accel.x = (float)raw * local->accel_coef;
    raw = ((int16_t)accel[2]<<8 | (int16_t)accel[3]);
    local->accel.y = (float)raw * local->accel_coef;
    raw = ((int16_t)accel[4]<<8 | (int16_t)accel[5]);
    local->accel.z = (float)raw * local->accel_coef;
    //Calculate total vector
    local->acc_total = sqrtf(local->accel.x * local->accel.x +
                            local->accel.y * local->accel.y +
                            local->accel.z * local->accel.z);
    //Calculate accel angles
    local->acc_angle.x = asin(local->accel.x / local->acc_total) * (180 / PI);
    local->acc_angle.y = asin(local->accel.y / local->acc_total) * (180 / PI);
}

/**
 * @brief Calculation angle velocity from gyroscope raw data
 * @param local pointer to descriptor structure
 * @param gyro 6-byte array, gyroscope raw data
 * @param filter if is equal to true, data will be filtered
 */
void MPU6050_calculateGyro(MPU6050_TypeDef* local, uint8_t* gyro, bool filter)
{
    MPU6050_AxesConversion(gyro);
//    uint32_t metka;
//    metka = micros();
    //If filter enabled, this function takes 180+-4 mks (ftact=32MHz)
    local->pre_gyro.x = local->gyro.x;
    local->pre_gyro.y = local->gyro.y;
    local->pre_gyro.z = local->gyro.z;
    int16_t raw; 
    raw = ((int16_t)gyro[0]<<8 | (int16_t)gyro[1]);
    local->gyro.x = (float)raw * local->gyro_coef;
    raw = ((int16_t)gyro[2]<<8 | (int16_t)gyro[3]);
    local->gyro.y = (float)raw * local->gyro_coef;
    raw = ((int16_t)gyro[4]<<8 | (int16_t)gyro[5]);
    local->gyro.z = (float)raw * local->gyro_coef;
    if (!filter) return;
    local->gyro.x -= local->bias.x;
    local->gyro.y -= local->bias.y;
    local->gyro.z -= local->bias.z;
    local->gyro.x = MPU6050_Filtered(&local->filter_x, local->gyro.x);
    local->gyro.y = MPU6050_Filtered(&local->filter_y, local->gyro.y);
    local->gyro.z = MPU6050_Filtered(&local->filter_z, local->gyro.z);
//    metka = micros()-metka;
//    char anm[10];
//    ultoa(metka, anm, 10);
//    UART_Print(anm);
//    UART_Transmit('\t');
}

void MPU6050_CalibrateGyro(MPU6050_TypeDef* local)
{
    //clear eps values
    local->eps.x = 0.;
    local->eps.y = 0.;
    local->eps.z = 0.;
    local->bias.x = 0.;
    local->bias.y = 0.;
    local->bias.z = 0.;
    //????????? ??????? ???????? ????????? (????????)
    uint8_t raw[6];
    for (uint16_t i=0; i<200; i++)
    {
        readRegister(local->interface, MPU6050_GYRO_XOUT_H, raw+0);
        readRegister(local->interface, MPU6050_GYRO_XOUT_L, raw+1);
        readRegister(local->interface, MPU6050_GYRO_YOUT_H, raw+2);
        readRegister(local->interface, MPU6050_GYRO_YOUT_L, raw+3);
        readRegister(local->interface, MPU6050_GYRO_ZOUT_H, raw+4);
        readRegister(local->interface, MPU6050_GYRO_ZOUT_L, raw+5);
        MPU6050_calculateGyro(local, raw, false);
        local->bias.x += local->gyro.x;
        local->bias.y += local->gyro.y;
        local->bias.z += local->gyro.z;
        _delay_ms(50);
    }
    local->bias.x /= 200.;
    local->bias.y /= 200.;
    local->bias.z /= 200.;
    //????????? ??????? ???????? ??????????
    //TODO: ?????????? ?? ?????????????????? ??????????
    for (uint16_t i=0; i<200; i++)
    {
        readRegister(local->interface, MPU6050_GYRO_XOUT_H, raw+0);
        readRegister(local->interface, MPU6050_GYRO_XOUT_L, raw+1);
        readRegister(local->interface, MPU6050_GYRO_YOUT_H, raw+2);
        readRegister(local->interface, MPU6050_GYRO_YOUT_L, raw+3);
        readRegister(local->interface, MPU6050_GYRO_ZOUT_H, raw+4);
        readRegister(local->interface, MPU6050_GYRO_ZOUT_L, raw+5);
        MPU6050_calculateGyro(local, raw, false);
        local->eps.x += fabs(local->gyro.x - local->bias.x);
        local->eps.y += fabs(local->gyro.y - local->bias.y);
        local->eps.z += fabs(local->gyro.z - local->bias.z);
        _delay_ms(50);
    }
    local->eps.x /= 200.;
    local->eps.y /= 200.;
    local->eps.z /= 200.;
    //
    MPU6050_setFilterParameters(&local->filter_x, local->eps.x, local->eps.x, 0.002);
    MPU6050_setFilterParameters(&local->filter_y, local->eps.y, local->eps.y, 0.002);
    MPU6050_setFilterParameters(&local->filter_z, local->eps.z, local->eps.z, 0.002);
    //
    local->angle.x = 0.;
    local->angle.y = 0.;
    local->angle.z = 0.;
}

//==============================================================================
__attribute__((weak)) void MPU6050_AxesConversion(uint8_t* raw)
{
//    uint8_t data1, data2;
//    data1 = raw[0];
//    data2 = raw[1];
//    raw[0] = raw[4];
//    raw[1] = raw[5];
//    //Inversion
//    int16_t temp = ((int16_t)data1<<8)|data2;
//    temp ^= 0xFFFF;
//    temp += 1;
//    //Save result
//    raw[4] = temp>>8;
//    raw[5] = temp&0xFF;
    //Inversion
    int16_t temp = ((int16_t)raw[0]<<8)|raw[1];
    temp ^= 0xFFFF;
    temp += 1;
    //Save result
    raw[0] = temp>>8;
    raw[1] = temp&0xFF;
}