#include "System.h"

void LGT8F328P_ClockConfig()
{
    // By start: internal clock 32MHz clock, prescaler = 8
    // Set external 32MHz generator as clock input
    /* set to right prescale */
	CLKPR |= (1<<WCE);
    CLKPR = 0x01;
	asm volatile ("nop");
	asm volatile ("nop");
	/* enable external crystal */
	PMCR = (1<<PMCE);
    PMCR = (1<<OSCMEN)|(1<<RCMEN); //if WDT disabled
    //PMCR = (1<<OSCMEN)|(1<<RCMEN)|(1<RCKEN)|(1<<WCLKS); //if WDT enabled
	/* waiting for crystal stable */
    _delay_us(1);
	/* switch to external crystal */
	PMCR = (1<<PMCE);
    PMCR = (1<<CLKSS)|(1<<OSCMEN)|(1<<RCMEN); //if WDT disabled
    //PMCR = (1<<CLKSS)|(1<<OSCMEN)|(1<<RCMEN)|(1<RCKEN)|(1<<WCLKS);
	// waiting for crystal stable
    _delay_us(1);
	// set to right prescale
    //16 MHZ!!!
	CLKPR |= (1<<WCE);
    CLKPR = 0x1;
}

void UART_TX6()
{
    PMX0 |= (1<<WCE);
    PMX0 |= (1<<TXD6);
}

//==============================================================================
void System_Bip(uint8_t number)
{
#ifdef BEEPER_ENABLE
    for (uint8_t i=number; i!=0; i--)
    {
        __BUZZER_ON();
        _delay_ms(85);
        __BUZZER_OFF();
        _delay_ms(85);
    }
#endif
}

void System_BipError()
{
#ifdef BEEPER_ENABLE
    __BUZZER_ON();
    _delay_ms(1000);
    __BUZZER_OFF();
    _delay_ms(100);
#endif
}

//==============================================================================
void MPU_BMP_Init()
{
    bool res;
    //MPU initializing and calibration
#if MPU
    res = MPU6050_Init(&mpu, &mpu_interface, MPU_ADDRESS);
//#ifdef PROJECT_DEBUG
    UART_Print("MPU status: ");
    UART_Transmit(res+0x30);
    UART_Transmit('\n');
//#endif
    if (!res) System_BipError();
    else
    {
        MPU6050_setAccelScale(&mpu, ACCEL_SCALE_16G);
        MPU6050_setGyroScale(&mpu, GYRO_SCALE_2000);
        MPU6050_setBandwidth(&mpu, BANDWIDTH_94HZ);
    }
//#ifdef INITIALIZATION_SIGNALS
//    __LED_ON();
//#endif
//    MPU6050_CalibrateGyro(&mpu);
//#ifdef INITIALIZATION_SIGNALS
//    __LED_OFF();
//#endif
#endif

    //BMP0 init
#if BMP0
    res = BMP388_begin(&bmp0, &bmp0_interface, BMP0_ADDRESS);
//#ifdef PROJECT_DEBUG
    UART_Print("BMP0 status:");
    UART_Transmit(res+0x30);
    UART_Transmit('\n');
//#endif
    if (!res) System_BipError();
    else
    {
        BMP388_setTempOversampling(&bmp0, OVERSAMPLING_1X);
        BMP388_setPresOversampling(&bmp0, OVERSAMPLING_8X);
        BMP388_setIIRFilterCoeff(&bmp0, IIR_FILTER_COEF_3);
        BMP388_setOutputDataRate(&bmp0, ODR_50_HZ);
    }
#endif
    
    //BMP1 init
#if BMP1
    res = BMP388_begin(&bmp1, &bmp1_interface, BMP1_ADDRESS);
//#ifdef PROJECT_DEBUG
    UART_Print("BMP1 status:");
    UART_Transmit(res+0x30);
    UART_Transmit('\n');
//#endif
    if (!res) System_BipError();
    else
    {
        BMP388_setTempOversampling(&bmp1, OVERSAMPLING_1X);
        BMP388_setPresOversampling(&bmp1, OVERSAMPLING_8X);
        BMP388_setIIRFilterCoeff(&bmp1, IIR_FILTER_COEF_3);
        BMP388_setOutputDataRate(&bmp1, ODR_50_HZ);
    }
#endif
}


void MPU_BMP_Read()
{
#ifdef MPU
    MPU6050_calculateAccel(&mpu, Raw1.a);
    MPU6050_calculateGyro(&mpu, Raw1.g, true);
    MPU6050_calculateRPY(&mpu, deltat);
#endif
#if (BMP0 && !BMP1)
    BMP388_calculateTemperature(&bmp0, Raw1.t0);
    BMP388_calculatePressure(&bmp0, Raw1.p0);
    BMP388_calculateAltitude(&bmp0);
#endif
#if (BMP0 && BMP1)
    if (cycle_counter % 2 == 0)
    {
        BMP388_calculateTemperature(&bmp0, Raw1.t0);
        BMP388_calculatePressure(&bmp0, Raw1.p0);
        BMP388_calculateAltitude(&bmp0);
    }
    else
    {
        BMP388_calculateTemperature(&bmp1, Raw1.t1);
        BMP388_calculatePressure(&bmp1, Raw1.p1);
        BMP388_calculateAltitude(&bmp1);
    }
#endif
}

//==============================================================================

/**
 * @brief System_ReadCycleLog - read logs from disk to UART port
 * @param addr - number of cycle log, (0-65535)
 * @return no_empty - if false, this cycle log is empty
 */
bool System_ReadCycleLog(uint32_t addr)
{
    if (addr > 65535) return false;
    W25_Read128(addr<<7, cycle_log);
    uint32_t data_u;
    float data_f;
    char* u;
    //Machine Cycle
    u = (char*)&data_u;
    u[0] = cycle_log[0];
    u[1] = cycle_log[1];
    u[2] = cycle_log[2];
    u[3] = cycle_log[3];
    if (data_u == 0xFFFFFFFF) return false;
    ultoa(addr, arr, 10);
    UART_Print(arr);
    UART_Transmit(';');
    ultoa(data_u, arr, 10);
    UART_Print(arr);
    UART_Transmit(';');
    //deltat-altitude
    uint8_t p = 4;
    u = (char*)&data_f;
    for (uint8_t i=0; i<16; i++)
    {
        u[0] = cycle_log[p]; p+=1;
        u[1] = cycle_log[p]; p+=1;
        u[2] = cycle_log[p]; p+=1;
        u[3] = cycle_log[p]; p+=1;
        dtostrf(data_f, 6, 4, arr);
        UART_Print(arr);
        UART_Transmit(';');
        
    }
    //condition_mode
    utoa(cycle_log[68], arr, 10);
    UART_Print(arr);
    UART_Transmit(';');
    //twi_error
    utoa(cycle_log[69], arr, 10);
    UART_Print(arr);
    UART_Transmit(';');
    u = (char*)&data_u;
    u[0] = cycle_log[70];
    u[1] = cycle_log[71];
    u[2] = cycle_log[72];
    u[3] = cycle_log[73];
    ultoa(data_u, arr, 10);
    UART_Print(arr);
    UART_Transmit('\n');
    return true;
}


void System_ServoModify()
{
    __SERVOON_ON();
    Servo16_Print();
    _delay_ms(500);
    __SERVOON_OFF();
}

void System_ReadCommand()
{
    char* str = UART_ReadString();
    __LED_ON();
    if (strcmp(str, "SAP")==0)
    {
        servoa.bias += 200;
        System_ServoModify();
        goto Metka;
    }
    if (strcmp(str, "SAM")==0)
    {
        servoa.bias -= 200;
        System_ServoModify();
        goto Metka;
    }
    if (strcmp(str, "SBP")==0)
    {
        servob.bias += 200;
        System_ServoModify();
        goto Metka;
    }
    if (strcmp(str, "SBM")==0)
    {
        servob.bias -= 200;
        System_ServoModify();
        goto Metka;
    }
    if (strcmp(str, "SCP")==0)
    {
        servoc.bias += 200;
        System_ServoModify();
        goto Metka;
    }
    if (strcmp(str, "SCM")==0)
    {
        servoc.bias -= 200;
        System_ServoModify();
        goto Metka;
    }
    if (strcmp(str, "SDP")==0)
    {
        servod.bias += 200;
        System_ServoModify();
        goto Metka;
    }
    if (strcmp(str, "SDM")==0)
    {
        servod.bias -= 200;
        System_ServoModify();
        goto Metka;
    }
    if (strcmp(str, "SZERO")==0)
    {
        servoa.angle = 0;
        servob.angle = 0;
        servoc.angle = 0;
        servod.angle = 0;
        servoa.bias = 0;
        servob.bias = 0;
        servoc.bias = 0;
        servod.bias = 0;
        System_ServoModify();
        goto Metka;
    }
    if (strcmp(str, "SPRINT")==0)
    {
        System_ServoModify();
        goto Metka;
    }
    if (strcmp(str, "MPU_CALIB")==0)
    {
        MPU6050_CalibrateGyro(&mpu);
        goto Metka;
    }
    if (strcmp(str, "DISK_ERASE")==0)
    {
        W25_ChipErase();
        goto Metka;
    }
    if (strcmp(str, "DISK_READ")==0)
    {
//        for (uint16_t i=0; i<50; i++)
//        {
//            System_ReadCycleLog(i);
//        }
        uint32_t i = 0;
        while (System_ReadCycleLog(i)) {i += 1;}
        goto Metka;
    }
    if (strcmp(str, "DISK_VERIFY")==0)
    {
        UART_Print("Disk status: ");
        W25_VerifyConnection();
        UART_Transmit('\t');
        goto Metka;
    }
    if (strcmp(str, "ACTION")==0)
    {
        idle_mode = false;
        timer0_overflow_count = 0;
        TCNT0 = 0;
        goto Metka;
    }
    if (strcmp(str, "TWI_END")==0)
    {
        TWIend();
        goto Metka;
    }
    if (strcmp(str, "TWI_VERIFY")==0)
    {
        //Test MPU6050 ID
        uint8_t data;
        bool flag = readRegister(mpu.interface, MPU6050_WHO_AM_I, &data);
        UART_Print("MPU Status: ");
//        if (data == 0x68) UART_Transmit('1');
//        else UART_Transmit('0');
//        utoa(data, arr, 10);
//        UART_Print(arr);
        UART_Transmit(0x30 + flag);
        //Test BMP388 ID
#if BMP0
        flag |= readRegister(bmp0.interface, BMP388_CHIP_ID, &data);
        UART_Print("\tBMP0 Status: ");
        if (data == 0x50) UART_Transmit('1');
        else UART_Transmit('0');
#endif
#if BMP1
        flag |= readRegister(bmp1.interface, BMP388_CHIP_ID, &data);
        UART_Print("\tBMP1 Status: ");
        if (data == 0x50) UART_Transmit('1');
        else UART_Transmit('0');
#endif
        UART_Print("\tTWI: ");
        UART_Transmit(0x30 + flag);
        UART_Transmit('\t');
        goto Metka;
    }
    if (strcmp(str, "R0")==0)
    {
        for (uint32_t i=0; i<5000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R1")==0)
    {
        for (uint32_t i=5000UL; i<10000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R2")==0)
    {
        for (uint32_t i=10000UL; i<15000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R3")==0)
    {
        for (uint32_t i=15000UL; i<20000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R4")==0)
    {
        for (uint32_t i=20000UL; i<25000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R5")==0)
    {
        for (uint32_t i=25000UL; i<30000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R6")==0)
    {
        for (uint32_t i=30000UL; i<35000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R7")==0)
    {
        for (uint32_t i=35000UL; i<40000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R8")==0)
    {
        for (uint32_t i=40000UL; i<45000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R9")==0)
    {
        for (uint32_t i=45000UL; i<50000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R10")==0)
    {
        for (uint32_t i=50000UL; i<55000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R11")==0)
    {
        for (uint32_t i=55000UL; i<60000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R12")==0)
    {
        for (uint32_t i=60000UL; i<65000UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    if (strcmp(str, "R13")==0)
    {
        for (uint32_t i=65000UL; i<65536UL; i++)
        {
            System_ReadCycleLog(i);
        }
        goto Metka;
    }
    //Say "error" if the command cannot be decoded
    System_BipError();
    __LED_OFF();
    UART_Println("Command error");
    System_Bip(2);
    return;
Metka:
    __LED_OFF();
    UART_Println("Done.");
    System_Bip(2);
}

//==============================================================================
void System_LogData()
{
    char* u;
    //Machine Cycle
    u = (char*)&(cycle_counter);
    cycle_log[0] = *(u+0);
    cycle_log[1] = *(u+1);
    cycle_log[2] = *(u+2);
    cycle_log[3] = *(u+3);
    //deltat
    u = (char*)&(deltat);
    cycle_log[4] = *(u+0);
    cycle_log[5] = *(u+1);
    cycle_log[6] = *(u+2);
    cycle_log[7] = *(u+3);
    //gyro_x
    u = (char*)&(mpu.gyro.x);
    cycle_log[8] = *(u+0);
    cycle_log[9] = *(u+1);
    cycle_log[10] = *(u+2);
    cycle_log[11] = *(u+3);
    //gyro_y
    u = (char*)&(mpu.gyro.y);
    cycle_log[12] = *(u+0);
    cycle_log[13] = *(u+1);
    cycle_log[14] = *(u+2);
    cycle_log[15] = *(u+3);
    //gyro_z
    u = (char*)&(mpu.gyro.z);
    cycle_log[16] = *(u+0);
    cycle_log[17] = *(u+1);
    cycle_log[18] = *(u+2);
    cycle_log[19] = *(u+3);
    //accel_x
    u = (char*)&(mpu.accel.x);
    cycle_log[20] = *(u+0);
    cycle_log[21] = *(u+1);
    cycle_log[22] = *(u+2);
    cycle_log[23] = *(u+3);
    //accel_y
    u = (char*)&(mpu.accel.y);
    cycle_log[24] = *(u+0);
    cycle_log[25] = *(u+1);
    cycle_log[26] = *(u+2);
    cycle_log[27] = *(u+3);
    //accel_z
    u = (char*)&(mpu.accel.z);
    cycle_log[28] = *(u+0);
    cycle_log[29] = *(u+1);
    cycle_log[30] = *(u+2);
    cycle_log[31] = *(u+3);
    //roll
    u = (char*)&(mpu.angle.x);
    cycle_log[32] = *(u+0);
    cycle_log[33] = *(u+1);
    cycle_log[34] = *(u+2);
    cycle_log[35] = *(u+3);
    //pitch
    u = (char*)&(mpu.angle.y);
    cycle_log[36] = *(u+0);
    cycle_log[37] = *(u+1);
    cycle_log[38] = *(u+2);
    cycle_log[39] = *(u+3);
    //yaw
    u = (char*)&(mpu.angle.z);
    cycle_log[40] = *(u+0);
    cycle_log[41] = *(u+1);
    cycle_log[42] = *(u+2);
    cycle_log[43] = *(u+3);
    //temperature
    u = (char*)&(Temperature);
    cycle_log[44] = *(u+0);
    cycle_log[45] = *(u+1);
    cycle_log[46] = *(u+2);
    cycle_log[47] = *(u+3);
    //temperature
    u = (char*)&(Temperature);
    cycle_log[44] = *(u+0);
    cycle_log[45] = *(u+1);
    cycle_log[46] = *(u+2);
    cycle_log[47] = *(u+3);
    //st_pressure
    u = (char*)&(Pressure);
    cycle_log[48] = *(u+0);
    cycle_log[49] = *(u+1);
    cycle_log[50] = *(u+2);
    cycle_log[51] = *(u+3);
    //dyn_pressure
    u = (char*)&(DynPressure);
    cycle_log[52] = *(u+0);
    cycle_log[53] = *(u+1);
    cycle_log[54] = *(u+2);
    cycle_log[55] = *(u+3);
    //vspeed
    u = (char*)&(VSpeed);
    cycle_log[56] = *(u+0);
    cycle_log[57] = *(u+1);
    cycle_log[58] = *(u+2);
    cycle_log[59] = *(u+3);
    //dif_vspeed
    u = (char*)&(DifVSpeed);
    cycle_log[60] = *(u+0);
    cycle_log[61] = *(u+1);
    cycle_log[62] = *(u+2);
    cycle_log[63] = *(u+3);
    //altitude
    u = (char*)&(Altitude);
    cycle_log[64] = *(u+0);
    cycle_log[65] = *(u+1);
    cycle_log[66] = *(u+2);
    cycle_log[67] = *(u+3);
    //condition_mode
    cycle_log[68] = condition_mode;
    //twi_error
    cycle_log[69] = twi_error;
    //Machine Cycle
    uint32_t dummy = micros();
    u = (char*)&(dummy);
    cycle_log[70] = *(u+0);
    cycle_log[71] = *(u+1);
    cycle_log[72] = *(u+2);
    cycle_log[73] = *(u+3);
    
    //~~
    //Writing data
    W25_Write128(cycle_log);
}