#include "setup_ADC_timer.h"

void ADC_TIMER_init(void)
{
    // --- Setup ADC ---
    // Voltage Reference - AVcc with external capacitor at AREF
    ADMUX |= (0 << REFS1) | (1 << REFS0);
    // ADC Channel
    ADMUX |= (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0);
    // Prescaler (1/128)
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    //ADCSRA |= (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);


    // Enable Auto Trigger
    //ADCSRA |= (1 << ADATE);
    // ADC Auto Trigger Source - Timer0 Overflow
    //ADCSRB |= (1 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
    // ADC conversion complete interrupt
    //ADCSRA |= (1 << ADIE);


    // disconnect digital input from every ADC channel
    DIDR0 = 0xFF;
    // Enable ADC
    ADCSRA |= (1 << ADEN);

    // Start ADC
    //ADCSRA |= (1<<ADSC);


    // --- Setup TIMER0 ---
	//TCCR0A = 0;
	//TCCR0B = 0;
    
	//set pwm mode
    TCCR0A |=  (0 << WGM01) | (0 << WGM00); 
    TCCR0B |=  (0 << WGM02); 

    //set prescaler to 1024
    TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);

    //enable overflow interrupt
    TIMSK0 |= (1 << TOIE0);
}

uint16_t get_ADC_16(uint8_t channel)
{
    ADMUX &= 0xF8; // clear lower 3 MUX bits
    ADMUX |= (channel & 7); // now put in the MUX selections

    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1 << ADSC));
    return (uint16_t)(ADCL | (ADCH << 8));
}

uint8_t get_ADC_8(uint8_t channel)
{
    ADMUX &= 0xF8; // clear lower 3 MUX bits
    ADMUX |= (channel & 7); // now put in the MUX selections

    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1 << ADSC));
    return (uint8_t)ADCL; //tutaj moze bedzie trzeba jeszcze przeczytaj adch
}
