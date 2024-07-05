/* 
 * File:   Uartoma.h
 * Author: victor
 *
 * Created on May 8, 2024, 11:46 PM
 */

#ifndef UARTOMA_H
#define	UARTOMA_H

#include "defines.h"

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#if (F_CPU == 16000000UL)
#define UART_BAUD_115200_UBRRH      0
#define UART_BAUD_115200_UBRRL      8
#endif
#if (F_CPU == 32000000UL)
#define UART_BAUD_115200_UBRRH      0
#define UART_BAUD_115200_UBRRL      16
#endif
    
#define __UART_RXC_IRQ_Enable()     (UCSR0B |= (1<<RXCIE0))
#define __UART_RXC_IRQ_Disable()    (UCSR0B &= ~(1<<RXCIE0))

#define UART_RX_BUFFER_LENGTH   15

char UART_rx_buffer[UART_RX_BUFFER_LENGTH];
uint8_t UART_buf_pointer;
volatile bool UART_rx_ready;


void UART_INI();
//Transmitting
void UART_Transmit(char data);
void UART_Print(char *data);
void UART_Println(char* data);
//Receiving
char* UART_ReadString();

#endif	/* UARTOMA_H */

