/*
* PushButtonControls.c
*
* Created: 4/21/2014 12:00:51 AM
*  Author: Jocelyn Seal
*/

#include "PushButtonControls.h"
#include "Configuration.h"
#include "Chopper.h"

#include <asf.h>
#include <util/delay.h>

ISR(PORTD_INT1_vect)
{
    switch(PORTD.IN)
    {
        case SWITCH_TWO:
        {
            if(gProgramMode == PROGRAM_LASER_REP_RATE)
            {
                if(--gLaserRepetitionRate < LASER_RATE_MIN_HZ)
                {
                    gLaserRepetitionRate = LASER_RATE_MIN_HZ;
                }
            }
            
            else if(gProgramMode == PROGRAM_MULTIPLIER)
            {
                if(--gMultiplier < MULTIPLIER_MIN)
                {
                    gMultiplier = MULTIPLIER_MIN;
                }
            }
            
            break;
        }
        
        case SWITCH_THREE:
        {
            if(gProgramMode == PROGRAM_LASER_REP_RATE)
            {
                if(++gLaserRepetitionRate > LASER_RATE_MAX_HZ)
                {
                    gLaserRepetitionRate = LASER_RATE_MAX_HZ;
                }
            }
            
            else if(gProgramMode == PROGRAM_MULTIPLIER)
            {
                if(++gMultiplier > MULTIPLIER_MAX)
                {
                    gMultiplier = MULTIPLIER_MAX;
                }
            }
            
            break;
        }
        
        default:
        {
            break;
        }
    }
    
    LcdWriteChopperState(gLaserRepetitionRate,gMultiplier,gStateCounter);
}


// Interrupt service routine to
// handle switch input
ISR(PORTD_INT0_vect)
{
    
    if(PORTD.IN != SWITCH_ONE)
    {
        return;
    }
    
    gProgramMode = (ProgramModeEnum_t)(++gStateCounter);
    
    if(gProgramMode == STOP)
    {
        gStateCounter = 0;
    }
    
    int8_t idx = 0;
    for(;idx < NUM_LEDS; ++idx )
    {
        LED_On(LED0_GPIO + idx);
        _delay_ms(100);
    }
    
    idx = NUM_LEDS-1;
    for(;idx >=0; --idx )
    {
        LED_Off(LED0_GPIO + idx);
        _delay_ms(100);
    }
    
    LcdWriteChopperState(gLaserRepetitionRate,gMultiplier,gStateCounter);
}
