#include "Chopper.h"
#include "Configuration.h"
#include "Lcd.h"

void evsys_init(void)
{
   sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS);
   EVSYS.CH3MUX = EVSYS_CHMUX_TCC0_OVF_gc;
}

void tc_init(void)
{
   tc_enable(&TCC0);
   tc_set_wgm(&TCC0, TC_WG_NORMAL);
   tc_write_period(&TCC0, (sysclk_get_per_hz() / DAC_CONVERSION_RATE) - 1);
   tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1_gc);
}

void dac_init(void)
{
   struct dac_config conf;

   // Read current configuration to use as a starting point.
   dac_read_configuration(&CHOPPER_DAC, &conf);

   // Use AVCC as a voltage reference to get 3.3V max output.
   // Right adjust inputs to accept lower 12 bits of 16-bit input
   dac_set_conversion_parameters(&conf, DAC_REF_AVCC, DAC_ADJ_RIGHT);

   // Set DACA CH0 active for access on porta pin header
   dac_set_active_channel(&conf, CHOPPER_DAC_CHANNEL, 0);

   // Disable event triggering since we're setting the values 
   // manually
   dac_set_conversion_trigger(&conf, 0, 0);

   // Copied from example
#ifdef XMEGA_DAC_VERSION_1
   dac_set_conversion_interval(&conf, 1);
#endif

   // Write new configuration to DAC registers
   dac_write_configuration(&CHOPPER_DAC, &conf);

   // Setup the refresh rate
   dac_set_refresh_interval(&conf,5);

   // Turn on the DAC output
   dac_enable(&CHOPPER_DAC);

   // Initialize with 0 for output
   dac_set_channel_value(&CHOPPER_DAC, CHOPPER_DAC_CHANNEL, 0);
}


void LcdWriteChopperState(uint8_t laserRate,uint8_t multiplier, uint8_t state)
{
   SPI_t *device   = (SPI_t*)(0x08C0);
   device->CTRL    = 0xD2;
   device->INTCTRL = 0x00;

   if(state == 0)
   {
      LcdShowStartupScreen(device,STARTUP_DISPLAY_LINES);
      return;
   }

   char lines[LCD_ROWS][LCD_COLS];

   char* state_str[] = {
      "Initial",
      "PGM LASER", 
      "PGM MULT", 
      "START"
   };

   snprintf(lines[0],LCD_COLS,"  Laser : %02hhu",laserRate);
   snprintf(lines[1],LCD_COLS,"   Mult : %02hhu",multiplier);
   snprintf(lines[2],LCD_COLS,"Chopper : %04d Hz", laserRate*multiplier);
   snprintf(lines[3],LCD_COLS,"*** %s ***",state_str[state]);

   for(uint8_t idx=0; idx<LCD_ROWS; ++idx)
   {
      const char* line = lines[idx]; 
      LcdWriteData(device, line, strlen(line), idx+1, 1);
   }
}

uint16_t CalculateDacOutput(uint16_t chopperFrequency, int16_t error)
{
   const float CHOPPER_HZ_PER_STEP = 7.2160;
   const float DAC_ERROR_OFFSET = 30;
   return round(chopperFrequency*CHOPPER_HZ_PER_STEP + 30);
}

void StabilizeChopper()
{
   SPI_t *device   = (SPI_t*)(0x08C0);
   device->CTRL    = 0xD2;
   device->INTCTRL = 0x00;

   const uint8_t STABILIZATION_TIME_SEC = 30;
   const uint8_t HOURS = 0;
   const uint8_t MINUTES = 0;

   // compute chopper frequency
   const uint16_t chopper_frequency = gLaserRepetitionRate * gMultiplier;

   // calculate chopper frequency in steps used by DAC
   const uint16_t dac_output = CalculateDacOutput(chopper_frequency, 0);

   // write DAC voltage output to chopper
   dac_set_channel_value(&CHOPPER_DAC, CHOPPER_DAC_CHANNEL, dac_output);

   // Let system stabilize before making further frequency adjustments
   for(int8_t seconds = STABILIZATION_TIME_SEC; seconds > 0; --seconds)
   {
      LcdWriteLargeTime(device, HOURS, MINUTES, seconds);
      LcdWriteData(device,"*** STABILIZING ***", 19, 4, 1);
      _delay_ms(1000);
   }
}

