/*
 * PushButtonControls.h
 *
 * Created: 4/20/2014 11:59:34 PM
 *  Author: Jocelyn Seal
 */ 
#ifndef PUSHBUTTONCONTROLS_H_
#define PUSHBUTTONCONTROLS_H_

#include <stdint.h>

typedef enum {
    PROGRAM_LASER_REP_RATE = 1,
    PROGRAM_MULTIPLIER = 2,
    RUN = 3,
    STOP = 4
} ProgramModeEnum_t;

typedef enum {
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

ISR(PORTD_INT0_vect);
ISR(PORTD_INT1_vect);





#endif /* PUSHBUTTONCONTROLS_H_ */