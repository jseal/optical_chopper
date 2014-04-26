/*
 * PushButtonControls.h
 *
 * Created: 4/20/2014 11:59:34 PM
 *  Author: Jocelyn Seal
 */ 
#ifndef PUSHBUTTONCONTROLS_H_
#define PUSHBUTTONCONTROLS_H_

#include "Configuration.h"

typedef enum {
    SWITCH_ONE = 0x3E,
    SWITCH_TWO = 0x3D,
    SWITCH_THREE = 0x3B,
    SWITCH_FOUR = 0x37
} ButtonPushEnum_t;

ISR(PORTD_INT0_vect);
ISR(PORTD_INT1_vect);





#endif /* PUSHBUTTONCONTROLS_H_ */
