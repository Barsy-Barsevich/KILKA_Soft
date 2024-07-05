#include "System.h"

#if (MPU & !BMP0 & !BMP1)
ISR(TWI_vect)
{
    switch (twi_mode)
    {
        case 0:  twi_error = false; TWIstart(); break;
        case 1:  TWIwrite(MPU_ADDRESS<<1); break;
        case 2:  TWIwrite(MPU6050_ACCEL_XOUT_H); break;
        case 3:  TWIstart(); break;
        case 4:  TWIwrite((MPU_ADDRESS<<1)|1); break;
        case 5:  TWIrequest(true); break;
        case 6:  Raw1.a[0] = TWIread(); TWIrequest(true); break;
        case 7:  Raw1.a[1] = TWIread(); TWIrequest(true); break;
        case 8:  Raw1.a[2] = TWIread(); TWIrequest(true); break;
        case 9:  Raw1.a[3] = TWIread(); TWIrequest(true); break;
        case 10: Raw1.a[4] = TWIread(); TWIrequest(true); break;
        case 11: Raw1.a[5] = TWIread(); TWIrequest(true); break;
        case 12: Raw1.t[0] = TWIread(); TWIrequest(true); break;
        case 13: Raw1.t[1] = TWIread(); TWIrequest(true); break;
        case 14: Raw1.g[0] = TWIread(); TWIrequest(true); break;
        case 15: Raw1.g[1] = TWIread(); TWIrequest(true); break;
        case 16: Raw1.g[2] = TWIread(); TWIrequest(true); break;
        case 17: Raw1.g[3] = TWIread(); TWIrequest(true); break;
        case 18: Raw1.g[4] = TWIread(); TWIrequest(false); break;
        case 19: Raw1.g[5] = TWIread(); TWIend(); break;
    }
//    switch (twi_mode) {
//    case 0: TWIstart(); break;
//    case 1: TWIwrite(0x68<<1); break;
//    case 2: TWIwrite(MPU6050_WHO_AM_I); break;
//    case 3: TWIstart(); break;
//    case 4: TWIwrite((0x68<<1)|1); break;
//    case 5: TWIrequest(false); break;
//    case 6:
//      Raw1.a[0] = TWIread();
//      TWIend();
////      UART_PRINT("\nMPU6050 id = 0x");
////      UART_TRANSMIT(0x30 + (data>>4));
////      UART_TRANSMIT(0x30 + (data & 0x0F));
//      break;
//    }
    //Increment mode
    twi_mode += 1;
}
#endif
#if (MPU & (BMP0 & !BMP1))
ISR(TWI_vect)
{
    switch (twi_mode)
    {
        case 0:  TWIstart(); break;
        case 1:  twi_error = false; TWIwrite(MPU_ADDRESS<<1); break;
        case 2:  TWIwrite(MPU6050_ACCEL_XOUT_H); break;
        case 3:  TWIstart(); break;
        case 4:  TWIwrite((MPU_ADDRESS<<1)|1); break;
        case 5:  TWIrequest(true); break;
        case 6:  Raw1.a[0] = TWIread(); TWIrequest(true); break;
        case 7:  Raw1.a[1] = TWIread(); TWIrequest(true); break;
        case 8:  Raw1.a[2] = TWIread(); TWIrequest(true); break;
        case 9:  Raw1.a[3] = TWIread(); TWIrequest(true); break;
        case 10: Raw1.a[4] = TWIread(); TWIrequest(true); break;
        case 11: Raw1.a[5] = TWIread(); TWIrequest(true); break;
        case 12: Raw1.t[0] = TWIread(); TWIrequest(true); break;
        case 13: Raw1.t[1] = TWIread(); TWIrequest(true); break;
        case 14: Raw1.g[0] = TWIread(); TWIrequest(true); break;
        case 15: Raw1.g[1] = TWIread(); TWIrequest(true); break;
        case 16: Raw1.g[2] = TWIread(); TWIrequest(true); break;
        case 17: Raw1.g[3] = TWIread(); TWIrequest(true); break;
        case 18: Raw1.g[4] = TWIread(); TWIrequest(false); break;
        case 19: Raw1.g[5] = TWIread(); TWIstart(); break;
        case 20: TWIwrite(BMP0_ADDRESS<<1); break;
        case 21: TWIwrite(BMP388_PRESS_XLSB); break;
        case 22: TWIstart(); break;
        case 23: TWIwrite((BMP0_ADDRESS<<1)|1); break;
        case 24: TWIrequest(true); break;
        case 25: Raw1.p0[0] = TWIread(); TWIrequest(true); break;
        case 26: Raw1.p0[1] = TWIread(); TWIrequest(true); break;
        case 27: Raw1.p0[2] = TWIread(); TWIrequest(true); break;
        case 28: Raw1.t0[0] = TWIread(); TWIrequest(true); break;
        case 29: Raw1.t0[1] = TWIread(); TWIrequest(false); break;
        case 30: Raw1.t0[2] = TWIread(); TWIend(); break;
    }
    //Increment mode
    twi_mode += 1;
}
#endif
#if (MPU & (BMP0 & BMP1))
ISR(TWI_vect)//ISR(TWI_vect)
{
    switch (twi_mode)
    {
        case 0:  TWIstart(); break;
        case 1:  twi_error = false; TWIwrite(MPU_ADDRESS<<1); break;
        case 2:  TWIwrite(MPU6050_ACCEL_XOUT_H); break;
        case 3:  TWIstart(); break;
        case 4:  TWIwrite((MPU_ADDRESS<<1)|1); break;
        case 5:  TWIrequest(true); break;
        case 6:  Raw1.a[0] = TWIread(); TWIrequest(true); break;
        case 7:  Raw1.a[1] = TWIread(); TWIrequest(true); break;
        case 8:  Raw1.a[2] = TWIread(); TWIrequest(true); break;
        case 9:  Raw1.a[3] = TWIread(); TWIrequest(true); break;
        case 10: Raw1.a[4] = TWIread(); TWIrequest(true); break;
        case 11: Raw1.a[5] = TWIread(); TWIrequest(true); break;
        case 12: Raw1.t[0] = TWIread(); TWIrequest(true); break;
        case 13: Raw1.t[1] = TWIread(); TWIrequest(true); break;
        case 14: Raw1.g[0] = TWIread(); TWIrequest(true); break;
        case 15: Raw1.g[1] = TWIread(); TWIrequest(true); break;
        case 16: Raw1.g[2] = TWIread(); TWIrequest(true); break;
        case 17: Raw1.g[3] = TWIread(); TWIrequest(true); break;
        case 18: Raw1.g[4] = TWIread(); TWIrequest(false); break;
        case 19: Raw1.g[5] = TWIread(); TWIstart(); break;
    }
    if (cycle_counter % 2 == 0)
    {
        switch (twi_mode)
        {
            case 20: TWIwrite(BMP0_ADDRESS<<1); break;
            case 21: TWIwrite(BMP388_PRESS_XLSB); break;
            case 22: TWIstart(); break;
            case 23: TWIwrite((BMP0_ADDRESS<<1)|1); break;
            case 24: TWIrequest(true); break;
            case 25: Raw1.p0[0] = TWIread(); TWIrequest(true); break;
            case 26: Raw1.p0[1] = TWIread(); TWIrequest(true); break;
            case 27: Raw1.p0[2] = TWIread(); TWIrequest(true); break;
            case 28: Raw1.t0[0] = TWIread(); TWIrequest(true); break;
            case 29: Raw1.t0[1] = TWIread(); TWIrequest(false); break;
            case 30: Raw1.t0[2] = TWIread(); TWIend(); break;
        }
    }
    else 
    {
        switch (twi_mode)
        {
            case 20: TWIwrite(BMP1_ADDRESS<<1); break;
            case 21: TWIwrite(BMP388_PRESS_XLSB); break;
            case 22: TWIstart(); break;
            case 23: TWIwrite((BMP1_ADDRESS<<1)|1); break;
            case 24: TWIrequest(true); break;
            case 25: Raw1.p1[0] = TWIread(); TWIrequest(true); break;
            case 26: Raw1.p1[1] = TWIread(); TWIrequest(true); break;
            case 27: Raw1.p1[2] = TWIread(); TWIrequest(true); break;
            case 28: Raw1.t1[0] = TWIread(); TWIrequest(true); break;
            case 29: Raw1.t1[1] = TWIread(); TWIrequest(false); break;
            case 30: Raw1.t1[2] = TWIread(); TWIend(); break;
        }
    }
//Previous version: BMP0 & BMP1 data are read by each cycle
//    switch (twi_mode)
//    {
//        case 0:  TWIstart(); break;
//        case 1:  TWIwrite(MPU_ADDRESS<<1); break;
//        case 2:  TWIwrite(MPU6050_ACCEL_XOUT_H); break;
//        case 3:  TWIstart(); break;
//        case 4:  TWIwrite((MPU_ADDRESS<<1)|1); break;
//        case 5:  TWIrequest(true); break;
//        case 6:  Raw1.a[0] = TWIread(); TWIrequest(true); break;
//        case 7:  Raw1.a[1] = TWIread(); TWIrequest(true); break;
//        case 8:  Raw1.a[2] = TWIread(); TWIrequest(true); break;
//        case 9:  Raw1.a[3] = TWIread(); TWIrequest(true); break;
//        case 10: Raw1.a[4] = TWIread(); TWIrequest(true); break;
//        case 11: Raw1.a[5] = TWIread(); TWIrequest(true); break;
//        case 12: Raw1.t[0] = TWIread(); TWIrequest(true); break;
//        case 13: Raw1.t[1] = TWIread(); TWIrequest(true); break;
//        case 14: Raw1.g[0] = TWIread(); TWIrequest(true); break;
//        case 15: Raw1.g[1] = TWIread(); TWIrequest(true); break;
//        case 16: Raw1.g[2] = TWIread(); TWIrequest(true); break;
//        case 17: Raw1.g[3] = TWIread(); TWIrequest(true); break;
//        case 18: Raw1.g[4] = TWIread(); TWIrequest(false); break;
//        case 19: Raw1.g[5] = TWIread(); TWIstart(); break;
//        case 20: TWIwrite(BMP0_ADDRESS<<1); break;
//        case 21: TWIwrite(BMP388_PRESS_XLSB); break;
//        case 22: TWIstart(); break;
//        case 23: TWIwrite((BMP0_ADDRESS<<1)|1); break;
//        case 24: TWIrequest(true); break;
//        case 25: Raw1.p0[0] = TWIread(); TWIrequest(true); break;
//        case 26: Raw1.p0[1] = TWIread(); TWIrequest(true); break;
//        case 27: Raw1.p0[2] = TWIread(); TWIrequest(true); break;
//        case 28: Raw1.t0[0] = TWIread(); TWIrequest(true); break;
//        case 29: Raw1.t0[1] = TWIread(); TWIrequest(false); break;
//        case 30: Raw1.t0[2] = TWIread(); TWIstart(); break;
//        case 31: TWIwrite(BMP1_ADDRESS<<1); break;
//        case 32: TWIwrite(BMP388_PRESS_XLSB); break;
//        case 33: TWIstart(); break;
//        case 34: TWIwrite((BMP1_ADDRESS<<1)|1); break;
//        case 35: TWIrequest(true); break;
//        case 36: Raw1.p1[0] = TWIread(); TWIrequest(true); break;
//        case 37: Raw1.p1[1] = TWIread(); TWIrequest(true); break;
//        case 38: Raw1.p1[2] = TWIread(); TWIrequest(true); break;
//        case 39: Raw1.t1[0] = TWIread(); TWIrequest(true); break;
//        case 40: Raw1.t1[1] = TWIread(); TWIrequest(false); break;
//        case 41: Raw1.t1[2] = TWIread(); TWIend(); break;
//    }
    //Increment mode
    twi_mode += 1;
}
#endif

//==============================================================================
// <Interrupts>
ISR(TIMER2_COMPA_vect)
{
    //Error handling TWI
    TWIend();
    twi_error = true;
//#ifdef PROJECT_DEBUG
//    UART_Print("Error!\n");
//#endif
}
