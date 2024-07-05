#include "BMP388_C_Barsotion.h"

/**
 * @brief BMP388 initialization
 * @param local pointer to structure-descriptor
 * @param i pointer to interface structure-descriptor
 * @param addr device address (0x77 or 0x76)
 * @return if is not equal to true, initialization failed
 */
bool BMP388_begin(BMP388_TypeDef* local, Interface_TypeDef* i, uint8_t addr)
{
    local->address = addr;
    local->interface = i;
    Interface_Init(local->interface, local->address);
    uint8_t data;
    readRegister(local->interface, BMP388_CHIP_ID, &data);    
//#ifdef PROJECT_DEBUG
//    char anm[10];
//    itoa(data, anm, 10);
//    UART_Print(anm);
//#endif
    if (data != 0x50) return false;
    // Enable Thermometer & Barometer
    writeRegister(local->interface, BMP388_PWR_CTRL, 0x33);
    BMP388_readCalibrationData(local);
    return true;
}

/**
 * @brief Set temperature oversampling
 * @param local pointer to structure-descriptor
 * @param ovs oversampling value: OVERSAMPLING_1X or OVERSAMPLING_2X
 */
void BMP388_setTempOversampling(BMP388_TypeDef* local, BMP388_Oversampling_TypeDef ovs)
{
    local->osr_reg &= 0b00000111;
    local->osr_reg |= (ovs << 3);
    writeRegister(local->interface, BMP388_OSR, local->osr_reg);
}

/**
 * @brief Set pressure oversampling
 * @param local pointer to structure-descriptor
 * @param ovs oversampling value: 1X, 2X, 4X, 8X, 16X, 32X
 */
void BMP388_setPresOversampling(BMP388_TypeDef* local, BMP388_Oversampling_TypeDef ovs)
{
    local->osr_reg &= 0b00111000;
    local->osr_reg |= ovs;
    writeRegister(local->interface, BMP388_OSR, local->osr_reg);
}

/**
 * @brief Set IIR filter coefficient
 * @param local pointer to structure-descriptor
 * @param v coef value: IIR_FILTER_COEF_0, 1, 3, 7, 15, 31, 63, 127
 */
void BMP388_setIIRFilterCoeff(BMP388_TypeDef* local, BMP388_IIR_Filter_TypeDef v)
{
    writeRegister(local->interface, BMP388_CONFIG, v << 1);
}

/**
 * @brief Set output data rate
 * @param local pointer to structure-descriptor
 * @param v ODR_200_HZ, 100, 50, 25, 12p5, 6p25, 3p1, 1p5, 0p78, 0p39, 0p2,
 * 0p1, 0p05, 0p02, 0p01, 0p006, 0p003, 0p0015
 */
void BMP388_setOutputDataRate(BMP388_TypeDef* local, BMP388_ODR_TypeDef v)
{
    writeRegister(local->interface, BMP388_ODR, v);
}


void BMP388_readCalibrationData(BMP388_TypeDef* local) {
    uint8_t data[21];
    readRegister(local->interface, BMP388_T1_L, data+0);
    readRegister(local->interface, BMP388_T1_H, data+1);
    readRegister(local->interface, BMP388_T2_L, data+2);
    readRegister(local->interface, BMP388_T2_H, data+3);
    readRegister(local->interface, BMP388_T3,   data+4);
    readRegister(local->interface, BMP388_P1_L, data+5);
    readRegister(local->interface, BMP388_P1_H, data+6);
    readRegister(local->interface, BMP388_P2_L, data+7);
    readRegister(local->interface, BMP388_P2_H, data+8);
    readRegister(local->interface, BMP388_P3,   data+9);
    readRegister(local->interface, BMP388_P4,   data+10);
    readRegister(local->interface, BMP388_P5_L, data+11);
    readRegister(local->interface, BMP388_P5_H, data+12);
    readRegister(local->interface, BMP388_P6_L, data+13);
    readRegister(local->interface, BMP388_P6_H, data+14);
    readRegister(local->interface, BMP388_P7,   data+15);
    readRegister(local->interface, BMP388_P8,   data+16);
    readRegister(local->interface, BMP388_P9_L, data+17);
    readRegister(local->interface, BMP388_P9_H, data+18);
    readRegister(local->interface, BMP388_P10,  data+19);
    readRegister(local->interface, BMP388_P11,  data+20);
    //Get correction parameters
    local->par.T1 = (float)(((uint16_t)data[1] << 8) | ((uint16_t)data[0]));
    local->par.T2 = (float)(((uint16_t)data[3] << 8) | ((uint16_t)data[2]));
    local->par.T3 = (float)((int8_t)data[4]);
    local->par.P1 = (float)(((int16_t)data[6] << 8) | ((int16_t)data[5]));
    local->par.P2 = (float)(((int16_t)data[8] << 8) | ((int16_t)data[7]));
    local->par.P3 = (float)((int8_t)data[9]);
    local->par.P4 = (float)((int8_t)data[10]);
    local->par.P5 = (float)(((uint16_t)data[12] << 8) | ((uint16_t)data[11]));
    local->par.P6 = (float)(((uint16_t)data[14] << 8) | ((uint16_t)data[13]));
    local->par.P7 = (float)((int8_t)data[15]);
    local->par.P8 = (float)((int8_t)data[16]);
    local->par.P9 = (float)(((int16_t)data[18] << 8) | ((int16_t)data[17]));
    local->par.P10 = (float)((int8_t)data[19]);
    local->par.P11 = (float)((int8_t)data[20]);
    local->par.T1 =BMP388_AdvancedShift(local->par.T1, 8);
    local->par.T2 =BMP388_AdvancedShift(local->par.T2, -30);
    local->par.T3 =BMP388_AdvancedShift(local->par.T3, -48);
    local->par.P1 =BMP388_AdvancedShift(local->par.P1 - 16384.0, -20);
    local->par.P2 =BMP388_AdvancedShift(local->par.P2 - 16384.0, -29);
    local->par.P3 =BMP388_AdvancedShift(local->par.P3, -32);
    local->par.P4 =BMP388_AdvancedShift(local->par.P4, -37);
    local->par.P5 =BMP388_AdvancedShift(local->par.P5, 3);
    local->par.P6 =BMP388_AdvancedShift(local->par.P6, -6);
    local->par.P7 =BMP388_AdvancedShift(local->par.P7, -8);
    local->par.P8 =BMP388_AdvancedShift(local->par.P8, -15);
    local->par.P9 =BMP388_AdvancedShift(local->par.P9, -48);
    local->par.P10 =BMP388_AdvancedShift(local->par.P10, -48);
    local->par.P11 =BMP388_AdvancedShift(local->par.P11, -65);
}  

//Function AdvancedShift multiplies float f with 2^value
float BMP388_AdvancedShift(float f, uint8_t value)
{
    //escape from zero exception
    if (f == 0.0) return f;
    uint8_t* u = (uint8_t*)&f;
    //save the sign
    uint8_t sign = u[3] & 0x80;
    //highlight sign and exponent
    uint16_t exponent = ((uint16_t)u[3]<<8)|((uint16_t)u[2]);
    exponent += (uint16_t)value << 7;
    //clear sign bit
    exponent &= 0x7FFF;
    //add a native sign
    exponent |= (uint16_t)sign << 8;
    //save news
    u[2] = exponent & 0xFF;
    u[3] = exponent >> 8;
    return f;
}

//==============================================================================
void BMP388_readStaticPressure(BMP388_TypeDef* local)
{
    uint8_t t[3], p[3];
    //Read data
    readRegister(local->interface, BMP388_PRESS_XLSB, p+0);
    readRegister(local->interface, BMP388_PRESS_LSB,  p+1);
    readRegister(local->interface, BMP388_PRESS_MSB,  p+2);
    readRegister(local->interface, BMP388_TEMP_XLSB,  t+0);
    readRegister(local->interface, BMP388_TEMP_LSB,   t+1);
    readRegister(local->interface, BMP388_TEMP_MSB,   t+2);
    //Calculate data
    BMP388_calculateTemperature(local, t);
    BMP388_calculatePressure(local, p);
    BMP388_calculateAltitude(local);
    //Save result
    local->static_pressure = local->pressure;
    local->static_altitude = local->altitude;
}

//==============================================================================
inline float BMP388_compensateTemperature(BMP388_TypeDef* local, uint8_t* raw)
{
    uint32_t uncomp_temp = ((uint32_t)raw[2]<<16)|((uint32_t)raw[1]<<8)|raw[0];
    float data1;
    float data2;
    data1 = (float)uncomp_temp - local->par.T1;
    data2 = (float)data1 * local->par.T2;
    // Update the compensated temperature in calib structure since
    // this is needed for pressure calculation
    return data2 + (data1 * data1) * local->par.T3;
}
inline float BMP388_compensatePressure(BMP388_TypeDef* local, uint8_t* raw)
{
    uint32_t uncomp_press = ((uint32_t)raw[2]<<16)|((uint32_t)raw[1]<<8)|raw[0];
    // Temporary variables used for compensation
    float data1;
    float data2;
    float data3;
    float data4;
    float out1;
    float out2;
    float T_lin2 = local->temperature * local->temperature;
    float T_lin3 = T_lin2 * local->temperature;
    float F_uncomp_press = (float)uncomp_press;
    // Calibration data
    data1 = local->par.P6 * local->temperature;
    data2 = local->par.P7 * T_lin2;
    data3 = local->par.P8 * T_lin3;
    out1 = local->par.P5 + data1 + data2 + data3;
    data1 = local->par.P2 * local->temperature;
    data2 = local->par.P3 * T_lin2;
    data3 = local->par.P4 * T_lin3;
    out2 = F_uncomp_press * (local->par.P1 + data1 + data2 + data3);
    data1 = F_uncomp_press * F_uncomp_press;
    data2 = local->par.P9 + local->par.P10 * local->temperature;
    data3 = data1 * data2;
    data4 = data3 + (data1 * F_uncomp_press) * local->par.P11;
    return out1 + out2 + data4;
}


void BMP388_calculateTemperature(BMP388_TypeDef* local, uint8_t* raw)
{
    local->temperature = BMP388_compensateTemperature(local, raw);
}

void BMP388_calculatePressure(BMP388_TypeDef* local, uint8_t* raw)
{
    local->pressure = BMP388_compensatePressure(local, raw);
}

void BMP388_calculateAltitude(BMP388_TypeDef* local)
{
    local->altitude = 44330.0 * (1.0 - pow(local->pressure / _SEA_LEVEL, 0.1903));
}