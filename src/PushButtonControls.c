/*
* PushButtonControls.c
*
* Created: 4/21/2014 12:00:51 AM
*  Author: Jocelyn Seal
*/
#include "PushButtonControls.h"
#include "Configuration.h"
#include <asf.h>
#include <util/delay.h>

typedef enum ProgramMode {
    PROGRAM_LASER_REP_RATE = 1,
    PROGRAM_MULTIPLIER = 2,
    RUN = 3,
    STOP = 4
} ProgramModeEnum_t;

typedef enum ButtonPush {
    SWITCH_ONE = 0x3E,
    SWITCH_TWO = 0x3D,
    SWITCH_THREE = 0x3B
} ButtonPushEnum_t;

const static uint8_t NUM_LEDS = 8;
const static uint8_t MULTIPLIER_MIN = 1;
const static uint8_t MULTIPLIER_MAX = 20;
const static uint8_t LASER_RATE_MIN_HZ = 20;
const static uint8_t LASER_RATE_MAX_HZ = 50;

static ProgramModeEnum_t gProgramMode;
static uint8_t gLaserRepetitionRate = 20;
static uint8_t gMultiplier = 1;
static uint8_t gStateCounter = 0;


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
}
