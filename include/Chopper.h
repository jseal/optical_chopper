#ifndef CHOPPER_H
#define CHOPPER_H

#include <stdio.h>
#include <string.h>

#include "Lcd.h"
#include "PushButtonControls.h"

void evsys_init(void);
void tc_init(void);
void dac_init(void);
void LcdWriteChopperState(uint8_t laserRate,uint8_t multiplier, uint8_t state);
void StabilizeChopper();
uint16_t CalculateDacOutput(uint16_t chopperFrequency, int16_t error);

#endif
