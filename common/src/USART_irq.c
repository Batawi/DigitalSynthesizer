#include "USART_irq.h"

/* Static Variables */
static unsigned char USART_RxBuf[USART_RX_BUFFER_SIZE];
static volatile unsigned char USART_RxHead;
static volatile unsigned char USART_RxTail;
static unsigned char USART_TxBuf[USART_TX_BUFFER_SIZE];
static volatile unsigned char USART_TxHead;
static volatile unsigned char USART_TxTail;


static FILE mystdout = FDEV_SETUP_STREAM(USART_Transmit_IO, NULL, _FDEV_SETUP_WRITE);


//#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1


/* Initialize USART */
void USART_Init(void)
{
    unsigned int ubrr_val = MYUBRR;
	stdout = &mystdout; //overwriting standard output

	unsigned char x;

	/* Set the baud rate */
	UBRR0H = (unsigned char) (ubrr_val>>8);                  
	UBRR0L = (unsigned char) ubrr_val;
	
	/* Enable USART receiver and transmitter */
	UCSR0B = ((1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0)); 
	
	/* For devices in which UBRRH/UCSRC shares the same location
	* eg; ATmega16, URSEL should be written to 1 when writing UCSRC
	* 
	*/
	/* Set frame format: 8 data 1stop */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	/* Flush receive buffer */
	x = 0; 			    

	USART_RxTail = x;
	USART_RxHead = x;
	USART_TxTail = x;
	USART_TxHead = x;
}



ISR(USART_RX_vect)
{
	unsigned char data;
	unsigned char tmphead;

	/* Read the received data */
	data = UDR0;                 
	/* Calculate buffer index */
	tmphead = (USART_RxHead + 1) & USART_RX_BUFFER_MASK;
	/* Store new index */
	USART_RxHead = tmphead;

	if (tmphead == USART_RxTail)
	{
		/* ERROR! Receive buffer overflow */
	}
	
	/* Store received data in buffer */
	USART_RxBuf[tmphead] = data; 
}

ISR(USART_UDRE_vect)
{
	unsigned char tmptail;

	/* Check if all data is transmitted */
	if (USART_TxHead != USART_TxTail)
	{
		/* Calculate buffer index */
		tmptail = (USART_TxTail + 1) & USART_TX_BUFFER_MASK;
		/* Store new index */
		USART_TxTail = tmptail;      
		/* Start transmission */
		UDR0 = USART_TxBuf[tmptail];  
	}
	else
	{       
		/* Disable UDRE interrupt */
		UCSR0B &= ~(1<<UDRIE0);         
	}
}

unsigned char USART_Receive(void)
{
	unsigned char tmptail;
	
	/* Wait for incoming data */
	while (USART_RxHead == USART_RxTail);
	/* Calculate buffer index */
	tmptail = (USART_RxTail + 1) & USART_RX_BUFFER_MASK;
	/* Store new index */
	USART_RxTail = tmptail;                
	/* Return data */
	return USART_RxBuf[tmptail];          
}

void USART_Transmit(unsigned char data)
{
	unsigned char tmphead;
	
	/* Calculate buffer index */
	tmphead = (USART_TxHead + 1) & USART_TX_BUFFER_MASK;
	/* Wait for free space in buffer */
	while (tmphead == USART_TxTail);
	/* Store data in buffer */
	USART_TxBuf[tmphead] = data;
	/* Store new index */
	USART_TxHead = tmphead;               
	/* Enable UDRE interrupt */
	UCSR0B |= (1<<UDRIE0);                    
}

int USART_Transmit_IO(char data, FILE* stream)
{
	unsigned char tmphead;
	
	/* Calculate buffer index */
	tmphead = (USART_TxHead + 1) & USART_TX_BUFFER_MASK;
	/* Wait for free space in buffer */
	while (tmphead == USART_TxTail);
	/* Store data in buffer */
	USART_TxBuf[tmphead] = data;
	/* Store new index */
	USART_TxHead = tmphead;
	/* Enable UDRE interrupt */
	UCSR0B |= (1<<UDRIE0);

    return 0;
}


void USART_putstring(char* str)
{
	while(*str != 0x00)
	{
		USART_Transmit(*str);
		str++;
	}
}
