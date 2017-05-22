// Includes
#include <xc.h>    
#include "FunctionGenerator.h"
#include "connectionprotocol.h" 

void init_FG(void) {
    /*---------- 1. Pins Configuration ----------*/
    
    //configure FSYNC RB14 (RP14/AN14)
    ANSDbits.ANSD8 = 0;        // Digital Pin;
    TRISDbits.TRISD8 = 0;      // Output;
    PORTDbits.RD8 = 1;         // FSYNC = 1 -> STOP transmission (set high after transmission); = 0 -> START transmission (set low before transmission)
    
    // configure SDO RB15 (RP29/AN15)
    ANSDbits.ANSD10 = 0;        // Digital Pin;
    TRISDbits.TRISD10 = 0;      // Output;
    PORTDbits.RD10 = 0;         // Initial value
    
    // configure SDI RF4 (RP10/AN11)
    ANSBbits.ANSB1 = 0;         // Digital Pin;
    TRISBbits.TRISB1 = 1;       // Input;
    PORTBbits.RB1 = 0;          // Initial value
    
    // configure SCK RF5 (RP17/AN10)
    ANSDbits.ANSD9 = 0;         // Digital Pin;
    TRISDbits.TRISD9 = 0;       // Output;
    PORTDbits.RD9 = 1;          // Initial value (SCK is active low -> initial high)
    
    /*---------- 2. PPS configuration ----------*/
    
    // Now in main.file -> init_Chip())
    
    /*---------- 3. SPI configuration ----------*/
    
    // SPIxSTAT: SPIx STATUS AND CONTROL REGISTER (16 bits)
    SPI1STATbits.SPIEN = 0;     // Disable SPI;
    SPI1STATbits.SPITBF = 0;    // Clear transmission flag;
    SPI1STATbits.SPIROV  = 0;   // Clear read overflow flag
    
    // SPIXCON1: SPIx CONTROL REGISTER 1 (16 bits)
    SPI1CON1bits.DISSCK = 0;    // Internal SPI clock is enabled;
    SPI1CON1bits.DISSDO = 0;    // SDO pin is controlled by the module;
    SPI1CON1bits.MODE16 = 0;    // Communication is byte-wide (8 bits);
    SPI1CON1bits.SMP = 0;       // Input data is sampled at the middle of data output time;
    SPI1CON1bits.CKE = 1;       // Data is transmitted on falling clock edge; (Defined in the datasheet of AD9833)
    SPI1CON1bits.SSEN = 1;      // SSx pin is used by the module; pin is controlled by the port function;
    SPI1CON1bits.CKP = 1;       // Idle state for the clock is a high level; active state is a low level (Defined in the datasheet of AD9833);
    SPI1CON1bits.MSTEN = 1;     // Master mode;
    SPI1CON1bits.SPRE = 0b111;  // Secondary pre-scale 1:1;
    SPI1CON1bits.PPRE = 0;      // Primary pre-scale 64:1;
    
    //SPIxCON2: SPIx CONTROL REGISTER 2 (16 bits)
    SPI1CON2bits.FRMEN = 0;     // Framed SPIx support is disabled;
    SPI1CON2bits.SPIBEN = 0;    // Enhanced buffer is disabled (Legacy mode);
    
    SPI1STATbits.SPIEN   = 1;   // Enable SPI module.
}

unsigned char write_SPI(unsigned char data_out)
{
    char dummy;
    
    dummy = SPI1BUF;        // Dummy read of the SPI1BUF register to clear the SPIRBF flag
    SPI1BUF = data_out;     // Write command to buffer
    while( !SPI1STATbits.SPIRBF )               // Wait for the data to be sent out
        ;
  
    return 1;
}

void generator(void){ 
    unsigned long int freq = info.FG.freq; // n -> output of wanted frequency
    
    unsigned char LSB_UP, LSB_LOW, MSB_UP, MSB_LOW;     // For frequency register 
    unsigned char RESET_UP,RESET_LOW, UNRESET_UP, UNRESET_LOW;  // for control bits
    long int MSB,LSB;
    
    /********************************************/
    /* For Control bits                         */ 
    /********************************************/
    if(info.FG.wave == 0 )     // For square wave
    {
        RESET_UP      = 0b00100001;
        RESET_LOW     = 0b01101000;
        UNRESET_UP    = 0b00100000;
        UNRESET_LOW   = 0b01101000;
    }
    else                   // For sine wave
    {
        RESET_UP      = 0b00100001;
        RESET_LOW     = 0b00000000;
        UNRESET_UP    = 0b00100000;
        UNRESET_LOW   = 0b00000000;
    }
    
    
    /********************************************/
    /* For Frequency Register Bits              */ 
    /********************************************/
    if( freq < 10)             // Frequency output = 10Hz..10MHz
		freq = 10;
	
	if(freq > 1000000)
		freq = 1000000;
	
	freq = (long int)(RATIO * freq);          // FREQREG = (2^28 / FMCLK) * Fout = RATIO * n
    
    LSB = freq % 16384;                    // 28/2=14, 2^14=16384 => get LSB(14bits)
	MSB = (freq - LSB) / 16384 + 16384;    // Frequency - LSB -> right shift 14 bits 
                                        // -> MSB (14bits) -> +16384 -> [01]XXXXXXXXXXXXXX => MSB(16bits)
	LSB = LSB + 16384;                  // => LSB(16bits)
	
	LSB_LOW = LSB % 256;                // 8bits
	LSB_UP  = (LSB - LSB_LOW) / 256;    // 8bits
	
	MSB_LOW = MSB % 256;                //8bits
	MSB_UP  = (MSB - MSB_LOW) / 256;    //8bits
    
    /********************************************/
    /* For Data Transfer                        */ 
    /********************************************/
    // Control 16bits write in
    PORTDbits.RD8 = 0;     // Making FSYNC of AD9833 LOW before writing in
    //PORTGbits.RG9 = 0;
    write_SPI(RESET_UP);     // Reset(D8); Control(D15,D14); both LSB and MSB FREQ writes consecutively(D13);
    write_SPI(RESET_LOW);    // The internal clock is enabled(D7) ;the DAC has been put to sleep(D6);
                            // The output is to be a square wave (D5); the square wave frequency is not to be divided by two.
    
    // FREQ register LSB write in
    write_SPI(LSB_UP);   //LSB
    write_SPI(LSB_LOW);
    
    // FREQ register MSB write in
    write_SPI(MSB_UP);   //MSB
    write_SPI(MSB_LOW);
    
    // PHASE register write in
    write_SPI(0b11000000);   //writing to the PHASE0 register (D15,D14,D13) (No need for phase shift -> all 0s)
    write_SPI(0b00000000);   //and is writing a twelve bit value of zero (D11-D0)
    
    // Activate the output (disable reset)
    write_SPI(UNRESET_UP);
    write_SPI(UNRESET_LOW); 
    //PORTGbits.RG9 = 1;
    PORTDbits.RD8 = 1;  // Making FSYNC of AD9833 HIGH after writing in
    
}

void SquareWave_10K(void)
{
    //Control 16bits write in
    PORTDbits.RD8 = 0;  	//Making FSYNC of AD9833 LOW before writing in
    
    write_SPI(0b00100001);   //Reset(D8); Control(D15,D14); both LSB and MSB FREQ writes consecutively(D13);
    write_SPI(0b01101000);   // the internal clock is enabled(D7) ;the DAC has been put to sleep(D6);
                            // the output is to be a square wave (D5); the square wave frequency is not to be divided by two(D3).
   
    //FREQ register LSB write in
   
    write_SPI(0b01011000);   //LSB
    write_SPI(0b10010011);
   
    //FREQ register MSB write in
    
    write_SPI(0b01000000);   //MSB
    write_SPI(0b00010000);
    
    //PHASE register write in
    
    write_SPI(0b11000000);   //writing to the PHASE0 register (D15,D14,D13) 
    write_SPI(0b00000000);   //and is writing a twelve bit value of zero (D11-D0)
    
    //activate the output (disable reset)
    
    write_SPI(0b00000000);
    write_SPI(0b01101000);
    PORTDbits.RD8 = 1;
   
}

