#include "Time.h"

void TIME_INI()
{
    timer0_overflow_count = 0;
//    //atmega128:
//    // Fast PWM mode, OC0 pin as usual pin, prescaler 128
//    TCCR0 = (0<<COM01)|
//            (0<<COM00)|
//            (1<<WGM01)|
//            (1<<WGM00)|
//            (1<<CS02)|
//            (0<<CS01)|
//            (1<<CS00);
//    TCNT0 = 0;
//    TIMSK &= ~(1<<OCIE0);
//    TIMSK |= (1<<TOIE0);
//    sei();
    //atmega328:
    TCCR0A = (0<<COM0A1)|
             (0<<COM0A0)|
             (0<<COM0B1)|
             (0<<COM0B0)|
             (1<<WGM01)|
             (1<<WGM00);
#if F_CPU == 16000000UL
    TCCR0B |= (0<<CS02)|(1<<CS01)|(1<<CS00); //prescaler 64
#endif
#if F_CPU == 32000000UL
    TCCR0B |= (1<<CS02)|(0<<CS01)|(0<<CS00); //prescaler 256
#endif
    TCNT0 = 0;
    TIMSK0 = 0b00000001;
    sei();
}

uint32_t micros()
{
    cli();
    uint32_t ans = timer0_overflow_count;
#if F_CPU == 16000000UL
    ans <<= 10;
    if (TIFR0 & (1<<TOIE0)) ans += 1024;
    ans |= ((uint32_t)TCNT0<<2);
#endif
#if F_CPU == 32000000UL
    ans <<= 11;
    if (TIFR0 & (1<<TOIE0)) ans += 2048;
    ans |= ((uint32_t)TCNT0<<3);
#endif
    sei();
    return ans;
}

uint32_t millis()
{
    return micros() / 1000;
}

ISR(TIMER0_OVF_vect)
{
    timer0_overflow_count += 1;
}
