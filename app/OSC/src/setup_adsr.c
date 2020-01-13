// -
// @file    setup_adsr.c
// -

#include "setup_adsr.h"

void setup_adsr(ADSR* adsr)
{
    adsr->a = 10;
    adsr->d = 20;
    adsr->s = 90;
    adsr->s_len = 15;
    adsr->r = 20;
    adsr->silencer = 0;
    adsr->time_counter = 1;
    adsr->step = 5;

    adsr->shifter = 0;
    adsr->numerator = SILENCER_MAX;
    adsr->numerator_shifter = 0;
    adsr->_denominator = &adsr->time_counter;
    adsr->current_step_len = 2;

}
void ADSR_change_step(ADSR* adsr)
{
    adsr->time_counter = 1;

    if(adsr->step != 5)
    {
        adsr->step++;
        switch(adsr->step)
        {
            case 0:
                break;
            case 1:     //Attack
                adsr->shifter = SILENCER_MAX;
                adsr->numerator = -SILENCER_MAX;
                adsr->numerator_shifter = 0;
                adsr->_denominator = &adsr->a;
                adsr->current_step_len = adsr->a;
                break;

            case 2:     //Decay
                adsr->shifter = 0;
                adsr->numerator = adsr->s;
                adsr->numerator_shifter = 0;
                adsr->_denominator = &adsr->d;
                adsr->current_step_len = adsr->d;
                break;

            case 3:     //Sustain
                adsr->shifter = 0;
                adsr->numerator = adsr->s;
                adsr->numerator_shifter = 0;
                adsr->_denominator = &adsr->time_counter;
                adsr->current_step_len = adsr->s_len;
                break;

            case 4:     //Release
                adsr->shifter = adsr->s;
                adsr->numerator = -adsr->s;
                adsr->numerator_shifter = SILENCER_MAX;
                adsr->_denominator = &adsr->r;
                adsr->current_step_len = adsr->r;
                break;

            case 5:     //After ADSR work
                adsr->shifter = 0;
                adsr->numerator = SILENCER_MAX;
                adsr->numerator_shifter = 0;
                adsr->_denominator = &adsr->time_counter;
                adsr->current_step_len = 2;
                break;
        }
    }
}
