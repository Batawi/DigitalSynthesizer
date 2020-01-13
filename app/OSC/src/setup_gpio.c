// -
// @file    setup_gpio.c
// @brief   setup general input output 
// -

#include "setup_gpio.h"

void setup_gpio(void)
{
    // wave selector
    GPIO_set_direction(PB, DDB0, INPUT);
    //GPIO_in_set_pullup(PB, DDB0, USE_PULLUP);
    GPIO_enable_pcint(PB, DDB0);

    // oscillator switch
    GPIO_set_direction(PD, DDD6, INPUT);
    //GPIO_in_set_pullup(PD, DDD6, USE_PULLUP);
    GPIO_enable_pcint(PD, DDD6);

    // filter switch
    GPIO_set_direction(PD, DDD5, INPUT);
    //GPIO_in_set_pullup(PD, DDD5, USE_PULLUP);
    GPIO_enable_pcint(PD, DDD5);

    // adsr switch
    GPIO_set_direction(PD, DDD4, INPUT);
    //GPIO_in_set_pullup(PD, DDD4, USE_PULLUP);
    GPIO_enable_pcint(PD, DDD4);

    // adsr set
    GPIO_set_direction(PD, DDD3, INPUT);
    //GPIO_in_set_pullup(PD, DDD3, USE_PULLUP);
    GPIO_enable_int(PD, DDD3, INTERRUPT_RISING_EDGE);

    // clock in
    GPIO_set_direction(PD, DDD2, INPUT);
    GPIO_in_set_pullup(PD, DDD2, USE_PULLUP);
    GPIO_enable_int(PD, DDD2, INTERRUPT_RISING_EDGE);
}
