// -
// @file    GPIO.c
// @brief   HAL for gpio features
// -

#include "GPIO.h"

void GPIO_set_direction(GPIO_port_t port, uint8_t pin, GPIO_direction_t dir)
{
    switch(port)
    {
        case PB:
            WRITE_BIT(DDRB, pin, dir);
            break;
        case PC:
            WRITE_BIT(DDRC, pin, dir);
            break;
        case PD:
            WRITE_BIT(DDRD, pin, dir);
            break;
    }
}

void GPIO_out(GPIO_port_t port, uint8_t pin, uint8_t value)
{
    switch(port)
    {
        case PB:
            WRITE_BIT(PORTB, pin, value);
            break;
        case PC:
            WRITE_BIT(PORTC, pin, value);
            break;
        case PD:
            WRITE_BIT(PORTD, pin, value);
            break;
    }
}

void GPIO_out_toggle(GPIO_port_t port, uint8_t pin)
{
    switch(port)
    {
        case PB:
            TOGGLE_BIT(PORTB, pin);
            break;
        case PC:
            TOGGLE_BIT(PORTC, pin);
            break;
        case PD:
            TOGGLE_BIT(PORTD, pin);
            break;
    }
}

void GPIO_in_set_pullup(GPIO_port_t port, uint8_t pin, GPIO_pull_t pullup)
{
    switch(port)
    {
        case PB:
            WRITE_BIT(PORTB, pin, pullup);
            break;
        case PC:
            WRITE_BIT(PORTC, pin, pullup);
            break;
        case PD:
            WRITE_BIT(PORTD, pin, pullup);
            break;
    }
}

uint8_t GPIO_in_read(GPIO_port_t port, uint8_t pin)
{
    switch(port)
    {
        case PB:
            return READ_BIT(PORTB, pin);
            break;
        case PC:
            return READ_BIT(PORTC, pin);
            break;
        case PD:
            return READ_BIT(PORTD, pin);
            break;
    }
    return 0;
}

void GPIO_enable_pcint(GPIO_port_t port, uint8_t pin)
{
    switch(port)
    {
        case PB:
            SET_BIT(PCICR, PCIE0);
            SET_BIT(PCMSK0, pin);
            break;
        case PC:
            SET_BIT(PCICR, PCIE1);
            SET_BIT(PCMSK1, pin);
            break;
        case PD:
            SET_BIT(PCICR, PCIE2);
            SET_BIT(PCMSK2, pin);
            break;
    }
}

void GPIO_disable_pcint(GPIO_port_t port, uint8_t pin)
{
    switch(port)
    {
        case PB:
            CLEAR_BIT(PCICR, PCIE0);
            CLEAR_BIT(PCMSK0, pin);
            break;
        case PC:
            CLEAR_BIT(PCICR, PCIE1);
            CLEAR_BIT(PCMSK1, pin);
            break;
        case PD:
            CLEAR_BIT(PCICR, PCIE2);
            CLEAR_BIT(PCMSK2, pin);
            break;
    }
}

void GPIO_enable_int(GPIO_port_t port, uint8_t pin, GPIO_interrupt_type_t type)
{
    if(port == PD && pin == 2)
    {
        EICRA |= type;
        SET_BIT(EIMSK, INT0);
    }
    else if(port == PD && pin == 3)
    {
        EICRA |= (type << 2);
        SET_BIT(EIMSK, INT1);
    }
}

void GPIO_disable_int(GPIO_port_t port, uint8_t pin, GPIO_interrupt_type_t type)
{
    if(port == PD && pin == 2)
    {
        CLEAR_BIT(EIMSK, INT0);
    }
    else if(port == PD && pin == 3)
    {
        CLEAR_BIT(EIMSK, INT1);
    }
}

/*
ISR(INT0_vect)      External Interrupt Request 0
ISR(INT1_vect)      External Interrupt Request 0
ISR(PCINT0_vect)    Pin Change Interrupt Request 0
ISR(PCINT1_vect)    Pin Change Interrupt Request 1
ISR(PCINT2_vect)    Pin Change Interrupt Request 2
*/
