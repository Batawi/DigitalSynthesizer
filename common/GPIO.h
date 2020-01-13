// -----------------------------------------------------------------------------
// @file    GPIO.h
// @brief   HAL for gpio features
// -----------------------------------------------------------------------------
#ifndef GPIO_H_ 
#define GPIO_H_

// -----------------------------------------------------------------------------
// Includes.
// -----------------------------------------------------------------------------
#include <avr/io.h>
#include "defines.h"

// -----------------------------------------------------------------------------
// Defines, enums, structs, types.
// -----------------------------------------------------------------------------
typedef enum GPIO_direction_t
{
	INPUT = 0, //Input pin
	OUTPUT = 1 //Output pin
}GPIO_direction_t;

typedef enum GPIO_pull_t
{
	NO_PULL = 0, //No pull resistor
	USE_PULLUP = 1 //Use pull up resistor
}GPIO_pull_t;

typedef enum GPIO_port_t
{
	PB, //Port B
	PC, //Port C
	PD  //Port D
}GPIO_port_t;

typedef enum GPIO_interrupt_type_t
{
    // On pin low level
	// @remark Only works on pins that have INTx functionality.
	// @remark Interrupt will keep triggering as long as low level is held.
	INTERRUPT_LOW_LEVEL = 0,

    //On any logic change. 
	INTERRUPT_TOGGLE = 1,

    // On pin falling edge
	// @remark Only works on pins that have INTx functionality.
	INTERRUPT_FALLING_EDGE = 2,

    // On pin rising edge
    // @remark Only works on pins that have INTx functionality.
	INTERRUPT_RISING_EDGE = 3

}GPIO_interrupt_type_t;

// -----------------------------------------------------------------------------
// Internal functions.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported functions.
// -----------------------------------------------------------------------------

// -
// Set direction of a pin.
// -
void GPIO_set_direction(GPIO_port_t port, uint8_t pin, GPIO_direction_t dir);

// -
// Set value on pin setup as output.
// -
void GPIO_out(GPIO_port_t port, uint8_t pin, uint8_t value);

// -
// Toggle value on pin setup as output.
// -
void GPIO_out_toggle(GPIO_port_t port, uint8_t pin);

// -
// Enable / Disable pull up resistor.
// To use this pin must be set as input.
// -
void GPIO_in_set_pullup(GPIO_port_t port, uint8_t pin, GPIO_pull_t pullup);

// -
// Return value of pin.
// To use this pin must be set as input.
// -
uint8_t GPIO_in_read(GPIO_port_t port, uint8_t pin);

// -
// Enable / Disable pin change interrupt.
// To use this pin must be set as input.
// -
void GPIO_enable_pcint(GPIO_port_t port, uint8_t pin);
void GPIO_disable_pcint(GPIO_port_t port, uint8_t pin);

// -
// Enable / Disable change interrupt (avaible on only 2 pins).
// To use this pin must be set as input.
// -
void GPIO_enable_int(GPIO_port_t port, uint8_t pin, GPIO_interrupt_type_t type);
void GPIO_disable_int(GPIO_port_t port, uint8_t pin, GPIO_interrupt_type_t type);


#endif // GPIO_H_
