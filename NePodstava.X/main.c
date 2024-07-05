/* 
 * File:   main.c
 * Author: victor
 *
 * Created on May 8, 2024, 1:15 AM
 */

#include "defines.h"
#include "System.h"

#include "Uartoma.h"
#include "Time.h"

int main();
void System_MainCycle();


int main()
{
    LGT8F328P_ClockConfig();
    /* UART init */
    UART_TX6();
    UART_INI();
    UART_Println("\nTransmission started");
    /* TWI init */
    TWIbegin();
    TWIend();
    TWIsetErrorTime(200);
    /* External pins init */
    __LED_INI();
    __BUZZER_INI();
    __SERVOON_INI();
    /* MPU BMP init */
    MPU_BMP_Init();
    /* Flash init */
    W25_Begin();
    /* Wait 2sec to permit reprogram device (before servo on) */
    System_Bip(1);
    _delay_ms(2000);
    /* Servo init */
    Servo16_Init();
    __SERVOON_ON();
    Servo16_Print();
    _delay_ms(1000);
    __SERVOON_OFF();
    /* BMP read static pressure */
#if BMP0
    BMP388_readStaticPressure(&bmp0);
#endif
#if BMP1
    BMP388_readStaticPressure(&bmp1);
    BMP_PresDiff = bmp1.pressure - bmp0.pressure;
#endif
    /* Ready signal */
    
//    __SERVOON_ON();
//    OCR3A = 3000;
//    OCR3B = 3000;
    System_Bip(3);
    
//    UART_Print("TCCR1A: ");
//    utoa(TCCR1A, arr, 10);
//    UART_Print(arr);
//    UART_Print("\tTCCR3A: ");
//    utoa(TCCR3A, arr, 10);
//    UART_Print(arr);
//    UART_Print("\tTCCR1B: ");
//    utoa(TCCR1B, arr, 10);
//    UART_Print(arr);
//    UART_Print("\tTCCR3B: ");
//    utoa(TCCR3B, arr, 10);
//    UART_Print(arr);
    
    
    /* Time init */
    TIME_INI();
    /* Global vars init */
    cycle_counter = 0;
    deltat = 0.001;
    pre_micros = 0;
    PreAltitude = bmp0.static_altitude;
    
    /* Enable interrupts */
    sei();
    
//==============================================================================
    //Idle mode: command receive
    idle_mode = true;
    while (idle_mode)
    {
        System_ReadCommand();
    }
    
    //Active mode: main cycle
    __SERVOON_ON();
    active_mode = true;
    while (active_mode)
    {
        System_MainCycle();
    }
    
    //After flight: idle mode, buzzer on
    while (1)
    {
        __BUZZER_ON();
        _delay_ms(3000);
        __BUZZER_OFF();
        _delay_ms(1000);
    }
}

//==============================================================================
void System_MainCycle()
{
    /* TWI start */
    //TWIstartQueue(&twi_mode);
    twi_mode = 1;
    TWIstart();
    
    /* Time calculations */
    uint32_t _micros = micros();
    deltat = (float)(_micros - pre_micros) * 0.000001;
    pre_micros = _micros;

    /* MPU and BMP data calculations */
    MPU_BMP_Read();
    
    /* Altitude calculations */
    System_AltitudeVSpeedCalculations();
    
    /* Correction RPY */
    if ((cycle_counter != 0) && (condition_mode == 0))
    {
        MPU6050_AngleCorrection(&mpu);
    }
       
//    UART_Print("\nCN: ");
//    itoa(cycle_counter, arr, 10);
//    UART_Print(arr);
//    
//    UART_Print("\tTime: ");
//    ultoa(micros(), arr, 10);
//    UART_Print(arr);
//    UART_Print("\tDT: ");
//    dtostrf(deltat, -8, 4, arr);
//    UART_Print(arr);
//    UART_Print("\tAT: ");
//    dtostrf(mpu.acc_total, -8, 4, arr);
//    UART_Print(arr);
//    
//    UART_Print("\tGX: ");
//    dtostrf(mpu.gyro.x, -8, 4, arr);
//    UART_Print(arr);
//    UART_Print(" GY: ");
//    dtostrf(mpu.gyro.y, -7, 2, arr);
//    UART_Print(arr);
//    UART_Print(" GZ: ");
//    dtostrf(mpu.gyro.z, -7, 2, arr);
//    UART_Print(arr);
//    
//    UART_Print(" Roll: ");
//    dtostrf(mpu.angle.x, -8, 4, arr);
//    UART_Print(arr);
//    UART_Print(" Pitch: ");
//    dtostrf(mpu.angle.y, -7, 2, arr);
//    UART_Print(arr);
//    UART_Print(" Yaw: ");
//    dtostrf(mpu.angle.z, -7, 2, arr);
//    UART_Print(arr);
//    UART_Print(" T: ");
//    dtostrf(Temperature, -7, 2, arr);
//    UART_Print(arr);
//    UART_Print("\tTWI_ERR: ");
//    UART_Transmit(twi_error+0x30);
    
    System_SAS();
    //_delay_us(10000);
    System_VerifyConditions();
    cycle_counter += 1;
}


//==============================================================================
void System_AltitudeVSpeedCalculations()
{
#if (BMP0 & !BMP1)
    Pressure = bmp0.pressure;
    Altitude = bmp0.altitude - bmp0.static_altitude;
    DifVSpeed = (Altitude - PreAltitude) / deltat;
    PreAltitude = Altitude;
#endif
#if (BMP0 & BMP1)
    if (cycle_counter % 2 == 0)
    {
        Pressure = bmp0.pressure;
        Altitude = bmp0.altitude - bmp0.static_altitude;
        DifVSpeed = (Altitude - PreAltitude) / (2*deltat);
        PreAltitude = Altitude;
    }
    else
    {
        Temperature = bmp1.temperature;
        DynPressure = bmp1.pressure;
        //Calculate TAS
        float truerho = ((Pressure*28.98)/(8.3145*(Temperature+273.15)))*0.001;
        VSpeed = sqrtf(abs((2*(DynPressure-BMP_PresDiff-Pressure))/truerho));
    }
#endif
}