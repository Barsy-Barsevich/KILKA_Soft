/* 
 * File:   SPIEX_Barsotion.h
 * Author: victor
 *
 * Created on May 12, 2024, 10:25 PM
 */

#ifndef SPIEX_BARSOTION_H
#define	SPIEX_BARSOTION_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

// (E) Barsotion KY
// SPIEX_Barsotion v2.0 07.11.23
// Software SPI library by Barsy-Barsevich 

#include "defines.h"

//--<Define ports and bits>-------------------------------------------------
#define _PORT_MOSI_     PORTB
#define _PORT_MISO_     PORTB
#define _PORT_SCK_      PORTB
#define _PORT_SS_       PORTB
#define _BIT_MOSI_      3
#define _BIT_MISO_      4
#define _BIT_SCK_       5
#define _BIT_SS_        0

//--<SPI mode>-----------------------------------------------
#define SPI_MODE_00
//#define SPI_MODE_01
//#define SPI_MODE_10
//#define SPI_MODE_11


inline void SPI_down() { //ss_down
    asm volatile(
        "CBI %0,%1" "\n\t"
        :: "I" (_SFR_IO_ADDR(_PORT_SS_)) /* %0 */, "I" (_BIT_SS_) /* %1 */
    );
}
inline void SPI_up() { //ss_up
    asm volatile(
        "SBI %0,%1" "\n\t"
        :: "I" (_SFR_IO_ADDR(_PORT_SS_)) /* %0 */, "I" (_BIT_SS_) /* %1 */
    );
}

    
uint8_t SPI_ex(uint8_t data);

void SPI_activate();
void SPI_deactivate();


//#ifdef	__cplusplus
//}
//#endif

#endif	/* SPIEX_BARSOTION_H */

