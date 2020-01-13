#include "BLINK.h"

#define LED_DDR     DDRB
#define LED_PIN     PINB
#define LED         PB5

void BLINK_init(void)
{
    LED_DDR |= _BV(LED);
}

void BLINK_toggle(void)
{
    LED_PIN |= _BV(LED);
}
