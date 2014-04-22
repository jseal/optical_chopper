// Redefine Reported System clock frequency
// to 32 MHz if it's been set elsewhere.
#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#ifdef F_CPU
#undef F_CPU
#endif
#define F_CPU 32000000L

#include <asf.h>
#include <util/delay.h>

// Clock initialization function
void InitializeClock(void);

// Configure PushButton and Timer Interrupts
void ConfigureInterrupts(void);

#endif