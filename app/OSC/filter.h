// -----------------------------------------------------------------------------
// @file    filter.h
// -----------------------------------------------------------------------------
#ifndef FILTER_H_ 
#define FILTER_H_

// -----------------------------------------------------------------------------
// Includes.
// -----------------------------------------------------------------------------
#include <stdint.h>
#include <avr/io.h>
#include <stdlib.h>

#include "defines.h"
#include "waveforms_generator.h"


// -----------------------------------------------------------------------------
// Defines, enums, structs, types.
// -----------------------------------------------------------------------------

void setup_filter(void);
void morph_wave(uint8_t* wavetable, uint8_t* original_wavetable, uint8_t filtervalue);


#endif // FILTER_H_
