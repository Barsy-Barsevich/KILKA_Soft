#include "Servo16_C_Barsotion.h"

void Servo16_Init()
{
    //Pins init
    SERVOA_DDR |= (1<<SERVOA_PIN);
    SERVOB_DDR |= (1<<SERVOB_PIN);
    SERVOC_DDR |= (1<<SERVOC_PIN);
    SERVOD_DDR |= (1<<SERVOD_PIN);
    SERVOA_PORT &= ~(1<<SERVOA_PIN);
    SERVOB_PORT &= ~(1<<SERVOB_PIN);
    SERVOC_PORT &= ~(1<<SERVOC_PIN);
    SERVOD_PORT &= ~(1<<SERVOD_PIN);
    //TOP angles
    ICR1 = SERVO_TOP;
    ICR3 = SERVO_TOP;
    //Prescaler 8, Fast PWM, ICR-TOP
    TCCR1A = (1<<COM1A1)| //OCR C,D
             (0<<COM1A0)|
             (1<<COM1B1)|
             (0<<COM1B0)|
             (1<<WGM11)|
             (0<<WGM10);
    TCCR1B = (1<<WGM13)|
             (1<<WGM12)|
             (0<<CS12)| //presc=8
             (1<<CS11)|
             (0<<CS10);
    TCCR3A = (1<<COM3A1)| //OCR A,B
             (0<<COM3A0)|
             (1<<COM3B1)|
             (0<<COM3B0)|
             (0<<COM3C0)|
             (0<<COM3C1)|
             (1<<WGM31)|
             (0<<WGM30);
    TCCR3B = (1<<WGM33)|
             (1<<WGM32)|
             (0<<CS32)| //presc=8
             (1<<CS31)|
             (0<<CS30);
    servoa.angle = 0;
    servob.angle = 0;
    servoc.angle = 0;
    servod.angle = 0;
    servoa.bias = 0;
    servob.bias = 0;
    servoc.bias = 0;
    servod.bias = 0;
}

void Servo16_Print()
{
    if (servoa.angle > 90.) servoa.angle = 90.;
    if (servoa.angle < -90.) servoa.angle = -90.;
    if (servob.angle > 90.) servob.angle = 90.;
    if (servob.angle < -90.) servob.angle = -90.;
    if (servoc.angle > 90.) servoc.angle = 90.;
    if (servoc.angle < -90.) servoc.angle = -90.;
    if (servod.angle > 90.) servod.angle = 90.;
    if (servod.angle < -90.) servod.angle = -90.;
    float value;
    value = __SERVO_ANGLE_TO_MKS(servoa.angle+90.);
    SERVOA_OCR = (int16_t)(__SERVO_MKS_TO_OCR(value)) + servoa.bias;
    value = __SERVO_ANGLE_TO_MKS(servob.angle+90.);
    SERVOB_OCR = (int16_t)(__SERVO_MKS_TO_OCR(value)) + servob.bias;
    value = __SERVO_ANGLE_TO_MKS(servoc.angle+90.);
    SERVOC_OCR = (int16_t)(__SERVO_MKS_TO_OCR(value)) + servoc.bias;
    value = __SERVO_ANGLE_TO_MKS(servod.angle+90.);
    SERVOD_OCR = (int16_t)(__SERVO_MKS_TO_OCR(value)) + servod.bias;
}