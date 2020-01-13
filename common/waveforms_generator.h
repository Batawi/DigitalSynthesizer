/*
 * Simple lib just for toggling bulid in LED (D13).
 */

#ifndef __WAVEFORMS_GENERATOR_H__
#define __WAVEFORMS_GENERATOR_H__

// Includes

#include <stdint.h>
#include <math.h>

// Prototypes
void generate_sin(uint8_t* wavetable);
void generate_tri(uint8_t* wavetable);
void generate_saw(uint8_t* wavetable);
void generate_sqr(uint8_t* wavetable);


#endif // __WAVEFORMS_GENERATOR_H__
