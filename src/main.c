#include "Configuration.h"
#include "Chopper.h"
#include "Dac.h"


int main (void)
{
	const uint32_t SHOW_SCREEN_DELAY_MSEC = 2000;

    board_init();
  
    // Initialize the system clock
    InitializeClock();
 
    // Setup interrupts
    ConfigureInterrupts();

    // Initialize LCD
    InitializeSpi();

	 // setup SPI device 
    SPI_t *device   = (SPI_t*)(0x08C0);
    device->CTRL    = 0xD2;
    device->INTCTRL = 0x00;

	 LcdShowStartupScreen(device,STARTUP_DISPLAY_LINES);
    
    _delay_ms(SHOW_SCREEN_DELAY_MSEC);
    
    /*evsys_init();*/
    /*tc_init();*/
    dac_init();
   
    while(true);
}    



