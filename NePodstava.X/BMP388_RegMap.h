/* 
 * File:   BMP388_RegMap.h
 * Author: victor
 *
 * Created on May 9, 2024, 4:07 PM
 */

#ifndef BMP388_REGMAP_H
#define	BMP388_REGMAP_H

#ifdef	__cplusplus
extern "C" {
#endif

//Main register map
#define BMP388_CHIP_ID          0x00
#define BMP388_ERR_REG          0x02
#define BMP388_STATUS           0x03
#define BMP388_DATA_0           0x04
#define BMP388_DATA_1           0x05
#define BMP388_DATA_2           0x06
#define BMP388_DATA_3           0x07
#define BMP388_DATA_4           0x08
#define BMP388_DATA_5           0x09
#define BMP388_PRESS_XLSB       0x04
#define BMP388_PRESS_LSB        0x05
#define BMP388_PRESS_MSB        0x06
#define BMP388_TEMP_XLSB        0x07
#define BMP388_TEMP_LSB         0x08
#define BMP388_TEMP_MSB         0x09
#define BMP388_SENSORTIME_0     0x0C
#define BMP388_SENSORTIME_1     0x0D
#define BMP388_SENSORTIME_2     0x0E
#define BMP388_SENSORTIME_3     0x0F
#define BMP388_EVENT            0x10
#define BMP388_INT_STATUS       0x11
#define BMP388_FIFO_LENGTH_0    0x12
#define BMP388_FIFO_LENGTH_1    0x13
#define BMP388_FIFO_DATA        0x14
#define BMP388_FIFO_WTM_0       0x15
#define BMP388_FIFO_WTM_1       0x16
#define BMP388_FIFO_CONFIG_1    0x17
#define BMP388_FIFO_CONFIG_2    0x18
#define BMP388_INT_CTRL         0x19
#define BMP388_IF_CONF          0x1A
#define BMP388_PWR_CTRL         0x1B
#define BMP388_OSR              0x1C
#define BMP388_ODR              0x1D
#define BMP388_CONFIG           0x1F
#define BMP388_CMD              0x7E
//Trimming coefficients
#define BMP388_T1_L             0x31
#define BMP388_T1_H             0x32
#define BMP388_T2_L             0x33
#define BMP388_T2_H             0x34
#define BMP388_T3               0x35
#define BMP388_P1_L             0x36
#define BMP388_P1_H             0x37
#define BMP388_P2_L             0x38
#define BMP388_P2_H             0x39
#define BMP388_P3               0x3A
#define BMP388_P4               0x3B
#define BMP388_P5_L             0x3C
#define BMP388_P5_H             0x3D
#define BMP388_P6_L             0x3E
#define BMP388_P6_H             0x3F
#define BMP388_P7               0x40
#define BMP388_P8               0x41
#define BMP388_P9_L             0x42
#define BMP388_P9_H             0x43
#define BMP388_P10              0x44
#define BMP388_P11              0x45

#ifdef	__cplusplus
}
#endif

#endif	/* BMP388_REGMAP_H */

