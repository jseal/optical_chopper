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

// Interrupt vector 0 for PORTD. Handles mode switching
// and LCD backlight controls
ISR(PORTD_INT0_vect)
{
   // debounce switch
   _delay_ms(500);

	// SW3 --> SWITCH_FOUR. Toggles LCD backlight.
	if(PORTD.IN == SWITCH_FOUR)
	{
		SPI_t *device   = (SPI_t*)(0x08C0);
		device->CTRL    = 0xD2;
		device->INTCTRL = 0x00;
		backLightEnabled = !backLightEnabled;
		LcdBacklight(device,backLightEnabled);
		return;
	}

	// increment the global state counter.
	gProgramMode = (ProgramModeEnum_t)(++gStateCounter);

   if(gProgramMode == STABILIZE_CHOPPER)
   {
          StabilizeChopper();
      return;
   }

	// if we reach the last state, start over.
	if(gProgramMode == STOP)
	{
		gStateCounter = 0;
	}

	// update LCD
	LcdWriteChopperState(gLaserRepetitionRate,gMultiplier,gStateCounter);
}

// Interrupt vector 1 for PORTD. Handles counter 
// incrementing and decrememting for the current 
// mode.
ISR(PORTD_INT1_vect)
{
	switch(PORTD.IN)
	{
		// SW1 --> SWITCH_TWO. This switch controls decrementing.
		case SWITCH_TWO:
			{
				switch(gProgramMode)
				{
					case PROGRAM_LASER_REP_RATE:
						{
							if(--gLaserRepetitionRate < LASER_RATE_MIN_HZ)
							{
								gLaserRepetitionRate = LASER_RATE_MIN_HZ;
							}
							break;
						}

					case PROGRAM_MULTIPLIER:
						{
							if(--gMultiplier < MULTIPLIER_MIN)
							{
								gMultiplier = MULTIPLIER_MIN;
							}
							break;
						}
				}
				break;
			}

			// SW2 --> SWITCH_THREE. This switch controls incrementing.
		case SWITCH_THREE:
			{
				switch(gProgramMode)
				{
					case PROGRAM_LASER_REP_RATE:
						{
							if(++gLaserRepetitionRate > LASER_RATE_MAX_HZ)
							{
								gLaserRepetitionRate = LASER_RATE_MAX_HZ;
							}
							break;
						}

					case PROGRAM_MULTIPLIER:
						{
							if(++gMultiplier > MULTIPLIER_MAX)
							{
								gMultiplier = MULTIPLIER_MAX;
							}
							break;
						}
				}
				break;
			}
	}

	// update the LCD
	LcdWriteChopperState(gLaserRepetitionRate,gMultiplier,gStateCounter);
}


