// Redefine Reported System clock frequency
// to 32 MHz if it's been set elsewhere.
#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#ifdef F_CPU
#undef F_CPU
#endif
#define F_CPU 32000000L

#include <stdint.h>
#include <asf.h>
#include <util/delay.h>

// Clock initialization function
void InitializeClock(void);

// Configure PushButton and Timer Interrupts
void ConfigureInterrupts(void);

// define startup display
const char* STARTUP_DISPLAY_LINES[];

typedef enum {
    PROGRAM_LASER_REP_RATE = 1,
    PROGRAM_MULTIPLIER = 2,
    RUN = 3,
    STOP = 4
} ProgramModeEnum_t;

const static uint8_t NUM_LEDS = 8;
const static uint8_t MULTIPLIER_MIN = 1;
const static uint8_t MULTIPLIER_MAX = 20;
const static uint8_t LASER_RATE_MIN_HZ = 20;
const static uint8_t LASER_RATE_MAX_HZ = 50;

extern ProgramModeEnum_t gProgramMode;
extern uint8_t gLaserRepetitionRate;
extern uint8_t gMultiplier;
extern uint8_t gStateCounter;
extern bool backLightEnabled;

#endif
