#include "Chopper.h"

void LcdWriteChopperState(uint8_t laserRate,uint8_t multiplier, uint8_t state)
{
    char lines[LCD_ROWS][LCD_COLS];
    char* state_str[] = {"Initial","PGM LASER", "PGM MULT", 
        "Starting System"};
    
    snprintf(lines[0],LCD_COLS,"  Laser : %02hhu",laserRate);
    snprintf(lines[1],LCD_COLS,"   Mult : %02hhu",multiplier);
    snprintf(lines[2],LCD_COLS,"Chopper : %04d Hz", laserRate*multiplier);
    snprintf(lines[3],LCD_COLS,"- %s -",state_str[state]);
    
    SPI_t *device   = (SPI_t*)(0x08C0);
    device->CTRL    = 0xD2;
    device->INTCTRL = 0x00;
    
    for(uint8_t idx=0; idx<LCD_ROWS; ++idx)
    {
        const char* line = lines[idx]; 
        LcdWriteData(device, line, strlen(line), idx+1, 1);
    }
}