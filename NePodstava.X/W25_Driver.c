#include "W25_Driver.h"


void W25_Begin()
{
    SPI_activate();
    W25_Address = 0;
#ifdef ERASE_DISK_BEFORE_START
    W25_ChipErase();
#endif
}

void W25_ChipErase()
{
    //Chip erasing
    // Write enabling
    SPI_down();
    SPI_ex(W25_Write_Enable);
    SPI_up();
    SPI_down();
    SPI_ex(W25_Chip_Erase);
    SPI_up();
    //Wait for the writing complete
    uint8_t flag = 1;
    while (flag == 1) {
        SPI_down();
        SPI_ex(W25_Read_SR1);
        flag = SPI_ex(0xFF) & 0x01;
        SPI_up();
    }
    _delay_ms(1);
    //Write disabling
    SPI_down();
    SPI_ex(W25_Write_Disable);
    SPI_up();
}

void W25_Write128(char *page) {
    if (W25_Address >= 0x800000) return;
    // Write enabling
    SPI_down();
    SPI_ex(W25_Write_Enable);
    SPI_up();
    // Sending the data
    SPI_down();
    SPI_ex(W25_Page_Program);
    SPI_ex((W25_Address>>16)&0xFF);
    SPI_ex((W25_Address>>8)&0xFF);
    SPI_ex(W25_Address & 0xFF);
    for (uint8_t i=0; i<128; i++) {
        SPI_ex(page[i]);
    }
    SPI_up();
    /*//Wait for the writing complete
    uint8_t flag = 1;
    while (flag == 1) {
        SPI_down();
        flag = SPI_ex(W25_Read_SR1) & 0x01;
        SPI_up();
    }*/
    //Write disabling
    _delay_ms(1);
    SPI_down();
    SPI_ex(W25_Write_Disable);
    SPI_up();
    //Special
    W25_Address += 128;
}


bool W25_VerifyConnection()
{
    uint8_t ManufacturerID, DeviceID;
    char anm[10];
    SPI_down();
    SPI_ex(W25_Manufacturer_Device_ID);
    SPI_ex(0);
    SPI_ex(0);
    SPI_ex(0);
    ManufacturerID = SPI_ex(0xFF);
    DeviceID = SPI_ex(0xFF);
    SPI_up();
    UART_Print("\tManufacturerID: ");
    itoa(ManufacturerID, anm, 16);
    UART_Print(anm);
    UART_Print("\tDeviceID: ");
    itoa(DeviceID, anm, 16);
    UART_Print(anm);
    if (ManufacturerID == 0xEF) return true;
    else return false;
}

void W25_Read128(uint32_t addr, char* buffer)
{
    SPI_down();
    SPI_ex(W25_Read_Data);
    SPI_ex((addr>>16)&0xFF);
    SPI_ex((addr>>8)&0xFF);
    SPI_ex(addr&0xFF);
    for (uint8_t i=0; i<128; i++)
    {
        buffer[i] = SPI_ex(0xFF);
    }
    SPI_up();
}