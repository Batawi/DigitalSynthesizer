// -
// @file    setup_dds.c
// -

#include "setup_dds.h"

//#define SAMPLE_RATE 24000UL

void setup_dds()
{
    //setup pwm timer
	TCCR2A = 0;
	TCCR2B = 0;
	
	TCCR2A |= _BV(WGM21) | _BV(WGM20); // Set fast PWM mode
	TCCR2A |= _BV(COM2A1); // Clear OC2A on Compare Match, set at BOTTOM
	TCCR2B |= _BV(CS20); // No prescaling
	OCR2A = 127; // Initial value
	DDRB |= _BV(PB3); // OC2A as output (pin 11)

    //setup sample time
	TCCR1A = 0;
	TCCR1B = 0;	
	TIMSK1 = 0;

	TCCR1B |= _BV(WGM12); // Set up in count-till-clear mode (CTC)
	TCCR1B |= _BV(CS10); // No prescaling
	TIMSK1 |= _BV(OCIE1A); // Enable output-compare interrupt
	OCR1A = F_CPU / SAMPLE_RATE - 1; // Set frequency
}
