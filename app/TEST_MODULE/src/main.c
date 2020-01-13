/*
connect buttons to pb0, pb1, pb2 (d8, d9, d10)
with resistors of course!
*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>    // Needed to use interrupts
#include "USART_irq.h"

int a, b, c;
volatile uint8_t portbhistory = 0xFF;     // default is high because the pull-up
//volatile uint8_t portbhistory = 0x00;     // default is high because the pull-up

int main(void)
{
    USART_Init();

    a = 0;
    b = 0;
    c = 0;

	DDRB |= (1<<PB5); //define output

    DDRB &= ~(1 << DDB0); // PB0 is now an input    
    DDRB &= ~(1 << DDB1); // PB1 is now an input    
    DDRB &= ~(1 << DDB2); // PB2 is now an input    

    PORTB |= (1 << PORTB0); // PB0 pull-up enabled
    PORTB |= (1 << PORTB1); // PB1 pull-up enabled
    PORTB |= (1 << PORTB2); // PB2 pull-up enabled
    
    PCICR |= (1 << PCIE0); // set PCIE0 to enable PCMSK0 scan
    PCMSK0 |= (1 << PCINT0); // set PCINT0 to trigger an interrupt on state change
    PCMSK0 |= (1 << PCINT1); // set PCINT1 to trigger an interrupt on state change
    PCMSK0 |= (1 << PCINT2); // set PCINT2 to trigger an interrupt on state change
	
    sei();
	while(1)
	{	
        printf("a: %d, b: %d, c: %d\r", a, b, c);
        _delay_ms(100);
	}
}

ISR (PCINT0_vect)
{
    uint8_t changedbits;

    changedbits = PINB ^ portbhistory;
    portbhistory = PINB;
    
    if(changedbits & (1 << PINB0))
    {
        PORTB |= (1<<PB5);
        a++;
    }
    
    else if(changedbits & (1 << PINB1))
    {
        PORTB |= (1<<PB5);
        b++;
    }

    else if(changedbits & (1 << PINB2))
    {
        PORTB |= (1<<PB5);
        c++;
    }
    /*
    if(PINB & (1 << PINB0))
    {
        a++;
    }
    else if(PINB & (1 << PINB1))
    {
        b++;
    }
    else if(PINB & (1 << PINB2))
    {
        c++;
    }
    */

    else
    {
        PORTB &= ~(1<<PB5);
    }	
    _delay_ms(50);
}
