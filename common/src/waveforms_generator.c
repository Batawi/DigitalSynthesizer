#include "waveforms_generator.h"

#define PI2 6.283185 // 2*PI saves calculation later
#define AMP 127 // Scaling factor for sine wave
#define OFFSET 128 // Offset shifts wave to all >0 values
#define LENGTH 256 // Length of the wave lookup table

void generate_sin(uint8_t* wavetable)
{
	// Populate the waveform table with a sine wave
	uint16_t i;
    float value;
	for(i = 0; i < LENGTH; i++) // Step across wave table
	{
		value = OFFSET + (AMP * sin((PI2 / LENGTH) * i)); // Compute value
		wavetable[i] = (uint8_t)value; // Store value as integer
	}
}

void generate_tri(uint8_t* wavetable)
{
	uint16_t i;
	for(i = 0; i < LENGTH; i++) // Step across wave table
	{
        if(i < (LENGTH / 2))
        {
            wavetable[i] = i*2;
        }
        else
        {
            wavetable[i] = (LENGTH-1) - (i*2);
        }
	}
}

void generate_saw(uint8_t* wavetable)
{
    uint16_t i;
	for(i = 0; i < LENGTH; i++) // Step across wave table
    {
        wavetable[i] = i;
    }
}

void generate_sqr(uint8_t* wavetable)
{
    uint16_t i;
	for(i = 0; i < LENGTH; i++) // Step across wave table
	{
        if(i < (LENGTH / 2))
        {
            wavetable[i] = 255;
        }
        else
        {
            wavetable[i] = 0;
        }
	}
}
