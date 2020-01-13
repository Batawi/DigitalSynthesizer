/* This is code for main module of modular synthesizer
 * Main module consist of:
 *     Oscillator (4 waveforms, frequency modulation, frequency potentiometer)
 *     Filter (morphing level)
 *     ADSR (ADSR and S_len potentiometers, clock in)
 *
 * Pins description:
 *     Output signal                PB3 (D11)
 *     Frequency pot                ADC7
 *     Filter pot                   ADC6
 *     S_len pot                    ADC4
 *     R pot                        ADC3
 *     S pot                        ADC2
 *     D pot                        ADC1
 *     A pot                        ADC0
 *
 *     Wave select BTN              PB0 (D8)
 *     Set ADSR pots BTN            PD3 (D3)
 *     
 *     Oscillator set / off SW      PD6 (D6)
 *     Filter on / off SW           PD5 (D5)
 *     ADSR on / off SW             PD4 (D4)
 *
 *
 *     --- Signals from another module ---
 *     FM in (analog)               ADC5
 *     Clock in (digital)           PD2 (D2)
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
//#include <avr/pgmspace.h>
//#include <stdio.h>
//#include <stdint.h>
//#include <util/delay.h>
#include <string.h>
//#include <stdlib.h>

#include "waveforms_generator.h"
#include "USART_irq.h"
#include "defines.h"
#include "GPIO.h"

#include "setup_ADC_timer.h"
#include "setup_gpio.h"
#include "setup_adsr.h"
#include "setup_dds.h"
#include "filter.h"


DDS voice;
ADSR adsr;

uint8_t wavetable[WAVETABLE_LEN]; //main wavetable, the one which is actually playing
uint8_t original_wavetable[WAVETABLE_LEN]; //pure wave, without any morphs

uint8_t filtervalue = 0; //this variable must be between -128 and 127
uint8_t prev_filtervalue = 1; //this variable must be between -128 and 127
uint8_t wave_selector = 0;



#define A4 440
#define A4_INDEX 127


// ON OFF flags
volatile uint8_t osc_on = 0;
volatile uint8_t filter_on = 0;
volatile uint8_t adsr_on = 0;

//volatile uint16_t adc_5 = 0;

int main(void)
{
    // Setups and inits functions
    setup_dds();
    setup_gpio();
    setup_filter();
    ADC_TIMER_init();
    setup_adsr(&adsr);
    adsr.silencer = 0;

    USART_Init();

    //blink init
    GPIO_set_direction(PB, 5, OUTPUT);

    // Initial values for DDS
    generate_sin(wavetable);
    memcpy(original_wavetable, wavetable, WAVETABLE_LEN);
	voice.accumulator = 0;
	//voice.increment = 500; //max value is 65535 (2 bytes), min=180, max=9000

    // Initial values of switches
    osc_on = READ_BIT(PIND, PIND6);
    filter_on = READ_BIT(PIND, PIND5);
    adsr_on = READ_BIT(PIND, PIND4);

    // Initial values for ADSR
    adsr.a = (uint8_t)(get_ADC_16(0) * 0.12414 + 2);
    adsr.d = (uint8_t)(get_ADC_16(1) * 0.12414 + 2);
    adsr.s = (uint8_t)(get_ADC_16(2) * 0.12414 + 2);
    adsr.r = (uint8_t)(get_ADC_16(3) * 0.12414 + 2);
    adsr.s_len = (uint8_t)(get_ADC_16(4) * 0.12414 + 2);

    // Initial value of adsr.increment
    voice.increment = get_ADC_16(7) * 8.6217 + 180;

    // Initial value for filtervalue
    filtervalue = (uint8_t)(get_ADC_16(6) * 0.24926);
    morph_wave(wavetable, original_wavetable, filtervalue);
    
    // FREQ table
    // increment = 120 --> freq = 43
    // increment = 150 --> freq = 54 
    // increment = 180 --> freq = 65
    // increment = 500 --> freq = 183
    // increment = 1000 --> freq = 366
    // increment = 2000 --> freq = 732
    // increment = 3000 --> freq = 1098
    // increment = 4000 --> freq = 1464
    // increment = 5000 --> freq = 1830
    // increment = 6000 --> freq = 2197
    // increment = 7000 --> freq = 2563
    // increment = 8000 --> freq = 2929
    // increment = 9000 --> freq = 3295



	sei(); // Set global interrupt flag
    //int debug_counter = 0;
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

        /*
        /ebug_counter++;
        if(debug_counter == 1000)
        {
            debug_counter = 0;

            printf("--- A = %d, D = %d, S = %d, R = %d, S_len = %d, Increment = %d, Filtervalue = %d ---\r",
                    adsr.a, adsr.d, adsr.s, adsr.r, adsr.s_len, voice.increment, filtervalue);

            //printf("--- osc_on: %d, filter_on: %d, adsr_on: %d, wave: %d ---\r", osc_on, filter_on, adsr_on, wave_selector);
            //printf("--- adc_5: %d ---\n\r", adc_5);

            //printf("adsr.step value: %d\n\r", adsr.step);
            //printf("%d\n\r", abs(adsr.silencer - 127));
            //printf("%d\n\r", voice.increment);
            //printf("%d\n\r", filtervalue);
            //printf("--- osc.sw: %d, filter.sw: %d, adsr.sw: %d ---\r", osc_on, filter_on, adsr_on);
        }
        */
	}
}

ISR(TIMER1_COMPA_vect) // Called when TCNT1 == OCR1A
{
	int16_t total = (uint8_t)wavetable[(voice.accumulator >> 8)];
	voice.accumulator += voice.increment;

    OCR2A = (total * (AMP_MID_POINT - adsr.silencer) / AMP_MID_POINT) + adsr.silencer;
}

ISR(TIMER0_OVF_vect) //This one was originaly used as ADC launcher
{
    //stop timer0
    TCCR0B |= (0 << CS02) | (0 << CS01) | (0 << CS00);
    
    //sei(); //it helps I DONT KNOW WHY!!!
    //OSC FREQ managemetn
    if(osc_on == 1)
    {
        voice.increment = get_ADC_16(7) * 8.6217 + 180;
    }

    //FILTER management
    if(filter_on == 1)
    {
        filtervalue = (uint8_t)(get_ADC_16(6) * 0.24926);
        if(filtervalue != prev_filtervalue)
        {
            prev_filtervalue = filtervalue;
            morph_wave(wavetable, original_wavetable, filtervalue);
        }
    }

    //ADSR management
    if(adsr_on == 1)
    {
        adsr.silencer = adsr.shifter + adsr.time_counter * (adsr.numerator + adsr.numerator_shifter) / *adsr._denominator;

        adsr.time_counter++;
        if(adsr.time_counter == adsr.current_step_len)
        {
            ADSR_change_step(&adsr);
        }
    }

    //start timer0
    TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);
}

ISR(INT0_vect)
{
    //Blink
    GPIO_out_toggle(PB, 5);


    //uint8_t midi_index;
    //uint16_t freq;

    //Clock in
    //starts adsr and reads FM analog input
    adsr.step = 0;
    ADSR_change_step(&adsr); // without this line also nice effect
    if(osc_on == 0)
    {
        //voice.increment = get_ADC_16(5) * 8.6217 + 180;
        //adc_5 = get_ADC_16(5);

        voice.increment = get_ADC_16(5) * 0.249266 + get_ADC_16(7) * 7;
        //voice.increment = get_ADC_16(5) * 0.249266 + 600;
        
        //midi_index = get_ADC_16(5) * 0.249266;
        //freq = pow(2, (midi_index - A4_INDEX) / 12) * A4; 
        //voice.increment = 2.730723 * freq + 1.690619;
    }
}

ISR(INT1_vect)
{
    //Read ADSR pots
    adsr.a = (uint8_t)(get_ADC_16(0) * 0.12414 + 2);
    adsr.d = (uint8_t)(get_ADC_16(1) * 0.12414 + 2);
    adsr.s = (uint8_t)(get_ADC_16(2) * 0.12414 + 2);
    adsr.r = (uint8_t)(get_ADC_16(3) * 0.12414 + 2);
    adsr.s_len = (uint8_t)(get_ADC_16(4) * 0.12414 + 2);
}

ISR(PCINT2_vect)
{
    osc_on = READ_BIT(PIND, PIND6);
    filter_on = READ_BIT(PIND, PIND5);
    adsr_on = READ_BIT(PIND, PIND4);

    if(adsr_on == 0)
    {
        adsr.silencer = 0;
    }
}

ISR(PCINT0_vect)
{
    if((PINB & (1 << PINB0)) == 1)
    {
        //Button as wave selector
        switch(wave_selector)
        {
            case 0:
                generate_sin(wavetable);
                wave_selector++;
                break;

            case 1:
                generate_tri(wavetable);
                wave_selector++;
                break;

            case 2:
                generate_saw(wavetable);
                wave_selector++;
                break;

            case 3:
                generate_sqr(wavetable);
                wave_selector = 0;
                break;
            default:
                generate_sin(wavetable);
        }

        memcpy(original_wavetable, wavetable, WAVETABLE_LEN);
        morph_wave(wavetable, original_wavetable, filtervalue);
    }
}
