#include "System.h"

void System_SAS()
{
    servoa.angle = -mpu.angle.z;
    servob.angle = -mpu.angle.y;
    servoc.angle = mpu.angle.z;
    servod.angle = mpu.angle.y;
    
    Servo16_Print();
}