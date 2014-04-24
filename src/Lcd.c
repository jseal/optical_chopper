#include "Lcd.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LcdWriteByte( SPI_t* device, register8_t data)
{
    PORTC.OUTCLR = PIN4_bm;
    _delay_us( CTRL_DELAY_US );

    device->DATA = data;

    bool complete = false;
    
    char data_read;
    
    while( !complete)
    {
        complete = device->STATUS == SPIF_BIT;
        data_read = device->DATA;
    };

    PORTC.OUTSET = PIN4_bm;
    _delay_us( CTRL_DELAY_US );
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void SleepMsec( const int msec )
{
    int i;
    for( i=0; i<msec; ++i)
    {
        _delay_us( 250 );
        _delay_us( 250 );
        _delay_us( 250 );
        _delay_us( 250 );
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LcdStartup( SPI_t* device, char* buffer, int size, int line )
{
    int i;

    /*LcdWaitForAvailableSpace(device, size);*/

    LcdWriteByte( device, LCD_SYNC_CMD );
    LcdWriteByte( device, LCD_STARTUP_CMD );
    LcdWriteByte( device, line );
    //
    for(i=0;i<size;++i)
    {
        LcdWriteByte( device, buffer[i] );
        SleepMsec( 10 );
    }

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LcdWaitForAvailableSpace( SPI_t* device, const int bytes )
{
    int space = 0;
    int counter = 0;
    register8_t data = 0x00;

    while( (space < bytes) && (counter < 255) )
    {
        LcdReadBufferSpace( device, &data );
        space = atoi( &data );
        _delay_ms( 1 );
        ++counter;
    }
}

void LcdReadBufferSpace( SPI_t* device, register8_t* data)
{
    const int BUFFER_MASK = 0x7f;

    PORTC.OUTCLR = PIN4_bm;
    _delay_us( CTRL_DELAY_US );

    LcdWriteByte( device, LCD_SYNC_CMD );
    LcdWriteByte( device, LCD_STATUS_CMD );
    LcdWriteByte( device, 0x00 );

    data = device->DATA & BUFFER_MASK;

    PORTC.OUTSET = PIN4_bm;
    _delay_us( CTRL_DELAY_US );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LcdClearDisplay( SPI_t* device )
{
    /*LcdWaitForAvailableSpace( device, 4 );*/

    LcdWriteByte( device, LCD_SYNC_CMD );
    LcdWriteByte( device,0x8c );
    LcdWriteByte( device, LCD_SYNC_CMD );
    LcdWriteByte( device,0x8b );
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LcdEnableDisplay( SPI_t* device, int state )
{
    int buffer = 0x12;

    if( state )
    {
        buffer = 0x1A;
    }
    
    /*LcdWaitForAvailableSpace( device, 2 );*/

    LcdWriteByte( device, LCD_SYNC_CMD );
    LcdWriteByte( device, buffer );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LcdBacklight( SPI_t* device, int state )
{
    int buffer = 0x20;

    if( state )
    {
        buffer = 0x28;
    }

    /*LcdWaitForAvailableSpace( device, 2);*/
    LcdWriteByte( device, LCD_SYNC_CMD );
    LcdWriteByte( device, buffer );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LcdWriteData(SPI_t* device, const char* buffer, uint8_t size,
uint8_t row, uint8_t column )
{
    uint8_t idx;

    /*LcdWaitForAvailableSpace(device, size);*/

    LcdWriteByte( device, LCD_SYNC_CMD );
    LcdWriteByte( device, LCD_POS_CMD );
    LcdWriteByte( device, row );
    LcdWriteByte( device , column);

    LcdWriteByte( device, LCD_SYNC_CMD );
    LcdWriteByte( device, LCD_WRITE_CMD );

    // write data to specificed row
    for( idx=0; idx<size; ++idx )
    {
        LcdWriteByte( device, buffer[idx] );
    }

    // pad remaining row with spaces.
    int offset = LCD_MAX_COLS - size - column + 1;
    for( idx=offset ; idx>0; --idx)
    {
        LcdWriteByte( device, 0x20 );
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//void InitializeClock()
//{
// setup the internal 32 MHz oscillator
//CLKSYS_Enable( OSC_RC32MEN_bm );
//CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_2_gc );
//do {} while ( CLKSYS_IsReady( OSC_RC32MRDY_bm ) == 0 );
//CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32M_gc );
//}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void InitializeSpi(void)
{
    PORTC.DIRSET = SPI_SS_bm | SPI_MOSI_bm | SPI_SCK_bm;
    PORTC.PIN4CTRL = PORT_OPC_WIREDANDPULL_gc;
    PORTC.OUTSET = PIN4_bm;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LcdWriteNumber( SPI_t* device, int row, int column, int size, int value )
{
    char buffer[255];
    sprintf( buffer, "%02u", value );
    LcdWriteData(device, buffer, size, row, column );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void SetLaserRepRate( SPI_t* device, int value )
{
    LcdWriteNumber( device, 1, 18, 2, value );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void SetClockFrequency( SPI_t* device, int value )
{
    LcdWriteNumber( device, 2, 18, 2, value );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void WriteVersionInfoToEeprom( SPI_t* device)
{
    // NOTE: The following commands will write permanent startup
    // data to the screen. This should be in a separate program
    // altogether.
    LcdStartup( device, "Arecibo Observatory ", 20, 1);
    LcdStartup( device, SOFTWARE_VERSION, 20, 2);
    LcdStartup( device, "                    ", 20, 3);
    LcdStartup( device, "                    ", 20, 4);
}
