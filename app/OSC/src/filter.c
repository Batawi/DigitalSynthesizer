// -
// @file    filter.c
// -

#include "filter.h"

uint8_t FILTERTABLE[WAVETABLE_LEN]; //boundry for nudging main wavetable during morphing

void setup_filter()
{
    generate_saw(FILTERTABLE);
}

void morph_wave(uint8_t* wavetable, uint8_t* original_wavetable, uint8_t filtervalue)
{
    uint16_t i;

    uint8_t distance;
    uint8_t modifying_step;

    for(i = 0; i<WAVETABLE_LEN; i++)
    {
        distance = abs(FILTERTABLE[i] - original_wavetable[i]);
        modifying_step = filtervalue * distance / WAVE_AMP;
        
        
        if(FILTERTABLE[i] > original_wavetable[i])
        {
            wavetable[i] = original_wavetable[i] + modifying_step; 
        }
        else if(FILTERTABLE[i] < original_wavetable[i])
        {
            wavetable[i] = original_wavetable[i] - modifying_step; 
        }
    }
}
