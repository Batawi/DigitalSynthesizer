/*
 * Simple lib just for toggling bulid in LED (D13).
 */

#ifndef __BLINK_H__
#define __BLINK_H__

// Includes

#include <avr/io.h>

// Prototypes
void BLINK_init(void);
void BLINK_toggle(void);


#endif // __BLINK_H__
