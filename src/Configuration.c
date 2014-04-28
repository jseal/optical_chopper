/*
* Initialize.c
*
* Created: 4/20/2014 8:12:00 PM
*  Author: Jocelyn Seal
*/

#include "Configuration.h"

ProgramModeEnum_t gProgramMode;
uint8_t gLaserRepetitionRate = 20;
uint8_t gMultiplier = 1;
uint8_t gStateCounter = 0;
bool backLightEnabled = true;

const char* STARTUP_DISPLAY_LINES[] = 
{
	"   Optical Chopper  " ,
	"Arecibo Observatory ",
	"Ver : 1.0.27Apr2014 ",
	" Press SW0 To Start"
};

void InitializeClock(void)
{
	// write to configuration protection register for
	// access to clock configuration
	// See Section 3.14.1 Configuration Change Protection
	CCP = 0xD8;

	// setup 2 clocks (one for system and one for
	// calibration):
	// 0x02 : 32 MHz internal oscillator
	// 0x04 : 32.768 kHz internal oscillator
	// See Section 7.9.1 CTRL - Control Register
	OSC.CTRL = 0x06;

	// setup prescalars A, B, and C
	// See Section 7.9.2 PSCTRL - Prescalar Register
	CLK_PSCTRL = 0x00;

	// Wait for enabled clocks to stabilize
	// See note about clock stabilization in
	// Section 7.9.1
	while(!(OSC.STATUS & 0x06))
	{
		_delay_us(100);
	}

	// write to configuration protection register for
	// access to clock configuration
	// See Section 3.14.1 Configuration Change Protection
	CCP = 0xD8;

	// select 32 MHz clock as system clock
	// See 7.9.1 CTRL - Control Register
	CLK_CTRL = 0x01;

	// write to configuration protection register for
	// access to clock configuration
	// See Section 3.14.1 Configuration Change Protection
	CCP = 0xD8;

	// Locks clock modifications for the duration of program
	// execution.
	CLK_LOCK = 0x01;
}

void ConfigureInterrupts(void)
{
	ioport_configure_pin(GPIO_PUSH_BUTTON_0, IOPORT_DIR_INPUT | IOPORT_PULL_UP | IOPORT_FALLING);
	ioport_configure_pin(GPIO_PUSH_BUTTON_1, IOPORT_DIR_INPUT | IOPORT_PULL_UP | IOPORT_FALLING);
	ioport_configure_pin(GPIO_PUSH_BUTTON_2, IOPORT_DIR_INPUT | IOPORT_PULL_UP | IOPORT_FALLING);
	ioport_configure_pin(GPIO_PUSH_BUTTON_3, IOPORT_DIR_INPUT | IOPORT_PULL_UP | IOPORT_FALLING);

	PORTD.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc;
	PORTD.INT0MASK = 0x09;
	PORTD.INT1MASK = 0x06;
	PMIC.CTRL = 0x01;

	sei();
}
