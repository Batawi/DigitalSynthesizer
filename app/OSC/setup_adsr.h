// -----------------------------------------------------------------------------
// @file    setup_adsr.h
// -----------------------------------------------------------------------------
#ifndef SETUP_ADSR_H_ 
#define SETUP_ADSR_H_

// -----------------------------------------------------------------------------
// Includes.
// -----------------------------------------------------------------------------
#include <stdint.h>
#include "defines.h"


// -----------------------------------------------------------------------------
// Defines, enums, structs, types.
// -----------------------------------------------------------------------------

typedef struct
{
    uint8_t a;
    uint8_t d;
    uint8_t s; //The range of the sustain is <0; 127>
    uint8_t s_len;
    uint8_t r;

    uint8_t silencer; //Output of the ADSR module
    uint8_t time_counter; //Increment by timer o overflow ISR
    uint8_t step; //A, D, S, R or nothing

    uint8_t shifter;
    uint8_t* _denominator;
    int8_t numerator;
    uint8_t numerator_shifter;
    uint8_t current_step_len;

}ADSR;

void setup_adsr(ADSR* adsr);
void ADSR_change_step(ADSR* adsr);


#endif // SETUP_ADSR_H_
