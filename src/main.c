#include "Configuration.h"
#include "Chopper.h"

int main (void)
{
    board_init();
  
    // Initialize the system clock
    InitializeClock();
 
    // Setup interrupts
    ConfigureInterrupts();
    
    // Initialize LCD
    InitializeSpi();
    
    while(true);
   
}    
    


