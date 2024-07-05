#include "System.h"

void System_VerifyConditions()
{
    switch (condition_mode)
    {
        //(0) Start waiting
        case 0:
            if ((mpu.acc_total > 3) && (Altitude > 4))
            {
                condition_mode += 1;
                apogee_detector_counter = 0;
            }
            //Log data once in 8 cycles
            if (cycle_counter % 8 == 0)
            {
                System_LogData();
            }
            break;
        //(1) Engine burnout, flight to the apogee
        case 1:
            if (DifVSpeed < -1) apogee_detector_counter += 1;
            else apogee_detector_counter = 0;
            if (apogee_detector_counter >= 10)
            {
                condition_mode += 1;
                landing_detector_counter = 0;
            }
            //Log data every cycle
            System_LogData();
            break;
        //(3) Flight by parashute to the landing
        case 2:
            //Writing only once by 32 cycles
            if ((mpu.acc_total < 1.5) && (Altitude < 10))
                landing_detector_counter += 1;
            else landing_detector_counter = 0;
            if (landing_detector_counter >= 10)
            {
                condition_mode += 1;
            }
            //Log data
            if (cycle_counter % 32 == 0)
            {
                System_LogData();
            }
            break;
        //(3) Waiting the rescue
        case 3:
            //Log data while there are empty sectors
            if (!(W25_Address >= 0x00800000) && (cycle_counter % 1024 == 0))
            {
                System_LogData();
            }
            break;
    }
}
