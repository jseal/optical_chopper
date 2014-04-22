#include "Configuration.h"
#include "PushButtonControls.h"

int main (void)
{
    board_init();
  
    // Initialize the system clock
    InitializeClock();
 
    // Setup interrupts
    ConfigureInterrupts();
    
    while(true);
 
}
