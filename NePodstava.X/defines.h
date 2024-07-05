/* 
 * File:   defines.h
 * Author: victor
 *
 * Created on June 24, 2024, 11:32 PM
 */

#ifndef DEFINES_H
#define	DEFINES_H

//#define F_CPU       32000000UL
#define F_CPU       16000000UL

#include <avr/io.h>
#include "lgtx8p.h"
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>
#include <avr/interrupt.h>

//==============================================================================
//TWI devices' addresses
//GammaAnomalain: mpu address 0x68
//DeltaAnomalain: mpu address 0x69
#define MPU_ADDRESS     0x69//0x68
#define BMP0_ADDRESS    0x77
#define BMP1_ADDRESS    0x76

//TWI devices configuration
#define MPU             true
//#define MPU             false
#define BMP0            true
//#define BMP0            false
//#define BMP1            true
#define BMP1            false


//==============================================================================
//Debug messages are sent by the UART port
#define PROJECT_DEBUG

//LED and BUZZER signals whereas initializing
#define INITIALIZATION_SIGNALS

//BUZZER sound enable
#define BEEPER_ENABLE

//Erase W25 chip before by the initialization
//#define ERASE_DISK_BEFORE_START

//==============================================================================
//Pin defines
#define LED_PORT        PORTD
#define LED_DDR         DDRD
#define LED_PIN         PD5
#define BUZZER_PORT     PORTD
#define BUZZER_DDR      DDRD
#define BUZZER_PIN      PD3
#define SERVOON_PORT    PORTD
#define SERVOON_DDR     DDRD
#define SERVOON_PIN     PD4
#define SERVOA_OCR      OCR3B
#define SERVOB_OCR      OCR3A
#define SERVOC_OCR      OCR1A
#define SERVOD_OCR      OCR1B
#define SERVOA_DDR      DDRF
#define SERVOB_DDR      DDRF
#define SERVOC_DDR      DDRB
#define SERVOD_DDR      DDRB
#define SERVOA_PORT     PORTF
#define SERVOB_PORT     PORTF
#define SERVOC_PORT     PORTB
#define SERVOD_PORT     PORTB
#define SERVOA_PIN      PF2
#define SERVOB_PIN      PF1
#define SERVOC_PIN      PB1
#define SERVOD_PIN      PB2
//LED, BUZZER and SERVO management
#define __LED_INI()     (LED_DDR |= (1<<LED_PIN))
#define __LED_ON()      (LED_PORT |= (1<<LED_PIN))
#define __LED_OFF()     (LED_PORT &= ~(1<<LED_PIN))
#define __BUZZER_INI()  (BUZZER_DDR |= (1<<BUZZER_PIN))
#define __BUZZER_ON()   (BUZZER_PORT |= (1<<BUZZER_PIN))
#define __BUZZER_OFF()  (BUZZER_PORT &= ~(1<<BUZZER_PIN))
#define __SERVOON_INI() (SERVOON_DDR |= (1<<SERVOON_PIN))
#define __SERVOON_ON()  (SERVOON_PORT |= (1<<SERVOON_PIN))
#define __SERVOON_OFF() (SERVOON_PORT &= ~(1<<SERVOON_PIN))

#endif	/* DEFINES_H */

