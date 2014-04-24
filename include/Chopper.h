#ifndef CHOPPER_H
#define CHOPPER_H

#include <stdio.h>
#include <string.h>

#include "Lcd.h"
#include "PushButtonControls.h"

void LcdWriteChopperState(uint8_t laserRate,uint8_t multiplier, uint8_t state);

#endif
