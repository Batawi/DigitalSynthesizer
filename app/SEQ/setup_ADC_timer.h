/*
 * This is wrapper for ADC triggered on overflow of timer0.
 * Connect potentiometer to (A0).
 */

#ifndef __ADC_TIMER_H__
#define __ADC_TIMER_H__

// Includes

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <stdio.h>

// Prototypes
void ADC_TIMER_init(void);
uint16_t get_ADC_16(uint8_t channel);
uint8_t get_ADC_8(uint8_t channel);

#endif // __ADC_TIMER_H__
