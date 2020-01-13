/*
 * This is wrapper for USART.
 * pritnf() is overwritten and can be used for debbuging.
 */
#ifndef __USART_IRQ_H__
#define __USART_IRQ_H__

// Includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>


// USART Buffer Defines
#define USART_RX_BUFFER_SIZE 8     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_TX_BUFFER_SIZE 8     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_RX_BUFFER_MASK (USART_RX_BUFFER_SIZE - 1)
#define USART_TX_BUFFER_MASK (USART_TX_BUFFER_SIZE - 1)

#if (USART_RX_BUFFER_SIZE & USART_RX_BUFFER_MASK)
#error RX buffer size is not a power of 2
#endif
#if (USART_TX_BUFFER_SIZE & USART_TX_BUFFER_MASK)
#error TX buffer size is not a power of 2
#endif


// Prototypes
//void USART_Init(unsigned int ubrr_val);
void USART_Init(void);
unsigned char USART_Receive(void);
void USART_Transmit(unsigned char data);
void USART_putstring(char* str);
int USART_Transmit_IO(char data, FILE* stream);



#endif // __USART_IRQ_H__
