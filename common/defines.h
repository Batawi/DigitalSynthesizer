#ifndef DEFINES_H_
#define DEFINES_H_

// Includes

// -
// Project related defines
// -
#define WAVE_AMP        255
#define WAVETABLE_LEN   256
#define AMP_MID_POINT   127 //Precise value is 127,5
#define SAMPLE_RATE     24000UL

#define SILENCER_MIN    0
#define SILENCER_MAX    127

// -
// Not project related defines
// -
// Basic bit manipulation macros
//Set bit y (0-indexed) of x to '1'
#define SET_BIT(x, y) (x |= (1 << y))

//Set bit y (0-indexed) of x to '0'
#define CLEAR_BIT(x, y) (x &= ~(1 << y))

//Return '1' if the bit value at position y within x is '1' and '0' if it's 0
#define READ_BIT(x, y) ((0u == (x & (1 << y))) ? 0u : 1u)

//Toggle bit y (0-index) of x
#define TOGGLE_BIT(x, y) (x ^= (1 << y))

//Write value z to bit y (0-index) of x 
#define WRITE_BIT(x, y, z) (z ? SET_BIT(x, y) : CLEAR_BIT(x, y))

// Binary values
#define LOW 0
#define HIGH 1

#define MAX(a,b)            (((a) > (b)) ? (a) : (b))

#define MIN(a,b)            (((a) < (b)) ? (a) : (b))

#endif // DEFINES_H_
