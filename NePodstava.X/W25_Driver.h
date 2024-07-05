/* 
 * File:   W25_Driver.h
 * Author: victor
 *
 * Created on June 25, 2024, 12:00 AM
 */

#ifndef W25_DRIVER_H
#define	W25_DRIVER_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

//<W25Qxx Commands>
#define W25_Write_Enable                0x06
#define W25_Volatile_SR_Write_Enable    0x50
#define W25_Write_Disable               0x04
#define W25_Read_SR1                    0x05
#define W25_Write_SR1                   0x01
#define W25_Read_SR2                    0x35
#define W25_Write_SR2                   0x31
#define W25_Read_SR3                    0x15
#define W25_Write_SR3                   0x11
#define W25_Chip_Erase                  0x60
#define W25_Power_Down                  0xB9
#define W25_Release_Power_Down_ID       0xAB
#define W25_Manufacturer_Device_ID      0x90
#define W25_JEDEC_ID                    0x9F
#define W25_Global_Block_Lock           0x7E
#define W25_Global_Block_Unlock         0x98
#define W25_Enter_QPI_Mode              0x38
#define W25_Enable_Reset                0x66
#define W25_Reset_Device                0x99
#define W25_Read_Unique_ID              0x4B
#define W25_Page_Program                0x02
#define W25_Quad_Page_Program           0x32
#define W25_Sector_Erase_4KB            0x20
#define W25_Sector_Erase_32KB           0x52
#define W25_Sector_Erase_64KB           0xD8
#define W25_Read_Data                   0x03
#define W25_Fast_Read                   0x0B

#include "defines.h"
#include "SPIEX_C_Barsotion.h"
#include "System.h"
#include "Uartoma.h"

uint32_t W25_Address;

void W25_Begin();
void W25_ChipErase();
void W25_Write128(char* page);
bool W25_VerifyConnection();
void W25_Read128(uint32_t addr, char* buffer);

//#ifdef	__cplusplus
//}
//#endif

#endif	/* W25_DRIVER_H */