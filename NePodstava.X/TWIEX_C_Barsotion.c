#include "TWIEX_C_Barsotion.h"

#ifdef TWI_ON_INTERRUPTS
void TWIbegin()
{
    //TWBR = 32;
    TWBR = TWI_TWBR();
    asm volatile("cli"); //cli();
    TCCR2A = 
        (0<<COM0A1)|
        (0<<COM0A0)|
        (0<<COM0B1)|
        (0<<COM0B0)|
        (1<<WGM01)| // CTC timer mode
        (0<<WGM00);
    TCCR2B = 
        (0<<FOC0A)|
        (0<<FOC0B)|
        (0<<WGM02)|
        (0<<CS22)| // 1/8 prescaling
        (0<<CS21)|
        (0<<CS20);
    OCR2A = 7; // 28us
    TIMSK2 = 
        (0<<OCIE0B)|
        (0<<OCIE0A)|  // disable twi timer
        (0<<TOIE0);
    asm volatile("sei"); //sei();
}
inline void TWIdisable_twi_timer()
{
    TIMSK2 = 0;
    TCCR2B = 0;
    TIFR2 = 0;
}
inline void TWIenable_twi_timer()
{
    TCNT2 = 0;
    TIMSK2 = (1<<OCIE0A);
    TCCR2B = 4;
}

#else
void TWI::begin()
{
    TWBR = TWI_TWBR();
}
#endif

bool TWIwriteRegister(uint8_t addr, uint8_t reg, uint8_t data)
{
    if (TWI_start() == false)                   return false;
    if (TWI_write(addr<<1) == false)            return false;
    if (TWI_write(reg) == false)                return false;
    if (TWI_write(data) == false)               return false;
    if (TWI_stop() == false)                    return false;
    return true;
}

bool TWIreadRegister(uint8_t addr, uint8_t reg, uint8_t* out)
{
    if (TWI_start() == false)                       return false;
    if (TWI_write(addr<<1) == false)                return false;
    if (TWI_write(reg) == false)                    return false; 
    if (TWI_start() == false)                       return false;
    if (TWI_write((addr<<1)|1) == false)   return false;
    if (TWI_read(false,out) == false)               return false;
    if (TWI_stop() == false)                        return false;
    return true;
}

bool TWI_start()
{
    //sei();
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    uint8_t counter = 0;
    bool flag = false;
    while (!flag && (counter < 80))
    {
        flag = ((TWCR & (1<<TWINT)) != 0);
        counter += 1;
    }
    return flag;
}

bool TWI_write(uint8_t data)
{
    TWDR = data;
    //sei();
    TWCR = (1<<TWINT)|(1<<TWEN);
    uint8_t counter = 0;
    bool flag = false;
    while (!flag && (counter < 160))
    {
        flag = ((TWCR & (1<<TWINT)) != 0);
        counter += 1;
    }
    return flag;
}

bool TWI_read(bool ACKM, uint8_t* data)
{
    TWDR = 0xFF;
    if (ACKM == true) TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    else              TWCR = (1<<TWINT)|(1<<TWEN);
    uint8_t counter = 0;
    bool flag = false;
    while (!flag && (counter < 160))
    {
        flag = ((TWCR & (1<<TWINT)) != 0);
        counter += 1;
    }
    *data = TWDR;
    return flag;
}

bool TWI_stop()
{
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
    uint8_t counter = 0;
    bool flag = false;
    while (!flag && (counter < 80))
    {
        flag = ((TWCR & (1<<TWINT)) != 0);
        counter += 1;
    }
    return true;
}


#ifdef TWI_ON_INTERRUPTS
/*inline*/ void TWIstartQueue(uint8_t* mode)
{
    *mode = 1;
    TWIstart();
}
/*inline*/ void TWIstart()
{
    //sei();
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) | (1<<TWIE);
    TWIenable_twi_timer();
}
/*inline*/ void TWIwrite(uint8_t data)
{
    TWDR = data;
    //sei();
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
    TWIenable_twi_timer();
}
/*inline*/ void TWIrequest(bool ACKM)
{
    TWDR = 0xFF;
    //sei();
    if (ACKM == true) TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE)|(1<<TWEA);
    else              TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);
    TWIenable_twi_timer();
}
/*inline*/ uint8_t TWIread()
{
    return TWDR;
}
/*inline*/ void TWIstop()
{
    //sei();
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN) | (1<<TWIE);
    TWIenable_twi_timer();
}
/*inline*/ void TWIend()
{
    //sei();
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN) | (1<<TWIE);
    TWIdisable_twi_timer();
}

/*
 * setErrorTime: set the time of TWI error
 *    void setErrorTime(uint8_t time_mks)
 * Input: maximal time of TWI answer waiting. If the TWI module does not answer
 * by the time > ErrorTime, microcontroller jumps to TIMER2_COMPA vector.
 * Minimal error_time value - 28mks. Maximal - 252mks. By default - 28mks
 * Output: nop
 */
void TWIsetErrorTime(uint8_t mks)
{
    mks >>= 2;
    if (mks < 7) OCR2A = 7;
    else OCR2A = mks;
}

#else
/*inline*/ void TWIend()
{
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN) | (1<<TWIE);
}
#endif