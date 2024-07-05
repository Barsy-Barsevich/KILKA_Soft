#include "MPU6050_C_Barsotion.h"

void MPU6050_calculateRPY(MPU6050_TypeDef* local, float dt)
{
    //Integral
    local->angle.x += local->gyro.x * dt;
    local->angle.y += local->gyro.y * dt;
    local->angle.z += local->gyro.z * dt;
    //Differencial
    local->angle_acceleration.x = (local->gyro.x - local->pre_gyro.x) / dt;
    local->angle_acceleration.y = (local->gyro.y - local->pre_gyro.y) / dt;
    local->angle_acceleration.z = (local->gyro.z - local->pre_gyro.z) / dt;
}

void MPU6050_AngleCorrection(MPU6050_TypeDef* local)
{
    local->angle.x = local->angle.x * 0.994 + local->acc_angle.x * 0.006;
    local->angle.y = local->angle.y * 0.994 + local->acc_angle.y * 0.006;
}