#include "Uartoma.h"

/**
 * @brief UART_INI - UART module initialization
 */
void UART_INI()
{
    /* Set baud rate */
//    UBRR0H = 0;
//    UBRR0L = 103;//19200
//    UBRR0H = ((uint16_t)UART_UBRR)>>8;
//    UBRR0L = UART_UBRR & 0xFF;
//    UBRR0H = UART_BAUD_115200_UBRRH;
//    UBRR0L = UART_BAUD_115200_UBRRL;
    UBRR0H = 0;
    UBRR0L = 0;
    
    /* Enable receiver and transmitter */
    UCSR0B = (1<<TXEN0)|(1<<RXEN0);
    /* Set frame format: 8data, 2stop bit */
    //UCSR0C = (1<<USBS)|(3<<UCSZ0);
    
    //
    UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);
    
    /* Reset rx variables */
    UART_buf_pointer = 0;
    UART_rx_ready = false;
}

/**
 * @brief UART_Transmit - transmitting 1 byte
 * @param data - data to send
 */
void UART_Transmit(char data)
{
/* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

/**
 * @brief UART_Print - printing a string to the UART port
 * @param data - pointer to string
 */
void UART_Print(char *data)
{
    for (uint8_t i=0; i<strlen(data); i++){
        UART_Transmit(data[i]);
    }
}

/**
 * @brief UART_Println - printing a string with "\n" to the UART port
 * @param data - pointer to string
 */
void UART_Println(char* data)
{
    UART_Print(data);
    UART_Transmit('\n');
}

/**
 * @brief UART_ReadString - receiving a string by UART port
 * @return a pointer to received string
 */
char* UART_ReadString()
{
    __UART_RXC_IRQ_Enable();
    while(!UART_rx_ready);
    UART_rx_ready = false;
    __UART_RXC_IRQ_Disable();
    for (uint8_t i=0; i<UART_RX_BUFFER_LENGTH; i++)
    {
        if (UART_rx_buffer[i] == 0x0A)
        {
            UART_rx_buffer[i] = 0x00;
            break;
        }
    }
    return UART_rx_buffer;
    //UART_Println(UART_rx_buffer);
}

//==============================================================================
ISR(USART_RX_vect)
{
    UART_rx_buffer[UART_buf_pointer] = UDR0;
    if (UART_rx_buffer[UART_buf_pointer] == 0x0A)
    {
        UART_rx_ready = true;
        UART_buf_pointer = 0;
    }
    else
    {
        UART_rx_ready = false;
        UART_buf_pointer += 1;
        if (UART_buf_pointer > UART_RX_BUFFER_LENGTH) UART_buf_pointer = 0;
    }
}