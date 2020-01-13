/* This is code for Clock + sequencer module
 * Pins description:
 */

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <math.h>
//#include <avr/pgmspace.h>
//#include <stdio.h>
//#include <stdint.h>
//#include <util/delay.h>
#include <string.h>
#include <stdlib.h> //abs()

#include <defines.h>
#include <setup_ADC_timer.h>
#include <GPIO.h>

volatile uint32_t clock_counter = 0;
volatile uint32_t clock_counter_max = 100;
volatile uint32_t clock_counter_short_period = 2;
volatile uint32_t clock_counter_long_period = 100;


volatile uint16_t seq_step = 0;
volatile uint16_t seq_freq[8];
#define MAX_SEQ_STEP 7

int main(void)
{
    // clock timer2 setup
    TCCR2A |= (0 << COM2A1) | (0 << COM2A0); //toggle OC2A on compare match
    TCCR2A |= (0 << WGM21) | (0 << WGM20); //normal mode
    TCCR2B |= (0 << WGM22); //normal mode
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // prescaler 1024
    TIMSK2 |= (1 << TOIE2); //enable overflow interrupt 

    // seq timer1 setup
    TCCR1A |= (0 << WGM11) | (1 << WGM10); //fast pwm 8 bit
    TCCR1B |= (0 << WGM13) | (1 << WGM12); //fast pwm 8 bit

    TCCR1A |= (1 << COM1A1) | (0 << COM1A0); //toggle OC2A on compare match

    TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10); // prescaler 1024
    OCR1A = 63;    
    GPIO_set_direction(PB, 1, OUTPUT);


    ADC_TIMER_init();

    //blink init
    GPIO_set_direction(PD, 7, OUTPUT);

    //setup rand()
    srand(get_ADC_16(0));
    uint8_t i;
    for(i = 0; i<= MAX_SEQ_STEP; i++)
    {
        seq_freq[i] = rand()%256;
    }


	sei(); // Set global interrupt flag
	while(1)
	{
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");


        //printf("--- A = %d, D = %d, S = %d, R = %d, S_len = %d, Increment = %d, Filtervalue = %d ---\r",
	}
}

ISR(TIMER1_COMPA_vect) // SEQ timer
{
}

ISR(TIMER2_OVF_vect) // Clock timer
{
    clock_counter++;
    if(clock_counter == clock_counter_max)
    {
        GPIO_out_toggle(PD, 7);

        if(clock_counter_max == clock_counter_short_period)
        {   
            // --- SEQ management ---
            OCR1A = seq_freq[seq_step];
            if(seq_step != MAX_SEQ_STEP)
            {
                seq_step++;
            }
            else
            {
                seq_step = 0;
            }
            // --- SEQ management END ---
            
           
            //max = 140, min = 10
            clock_counter_max = clock_counter_long_period;
        }
        else
        {
            clock_counter_max = clock_counter_short_period;
        }

        clock_counter = 0;
    }
}

ISR(TIMER0_OVF_vect) // ADC timer
{
    clock_counter_long_period = abs((int)(get_ADC_16(7) * 0.127077 - 130)) + 10; 
}

