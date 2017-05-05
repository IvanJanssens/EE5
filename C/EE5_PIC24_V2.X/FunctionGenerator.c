// Includes
#include <xc.h>
#include <PPS.h>                        // Include the header for PPS function    
#include "FunctionGenerator.h" 

void initFG(void)
{
    
    
    /*---------- 1. Pins Configuration ----------*/
    
    //configure FSYNC RB14 (RP14/AN14)
    ANSBbits.ANSB14 = 0;        // Digital Pin;
    TRISBbits.TRISB14 = 0;      // Output;
    PORTBbits.RB14 = 1;         // FSYNC = 1 -> STOP transmission (set high after transmission); = 0 -> START transmission (set low before transmission)
    
    // configure SDO RB15 (RP29/AN15)
    ANSBbits.ANSB15 = 0;        // Digital Pin;
    TRISBbits.TRISB15 = 0;      // Output;
    PORTBbits.RB15 = 0;         // Initial value
    
    // configure SDI RF4 (RP10/AN11)
    ANSFbits.ANSF4 = 0;         // Digital Pin;
    TRISFbits.TRISF4 = 1;       // Input;
    PORTFbits.RF4 = 0;          // Initial value
    
    // configure SCK RF5 (RP17/AN10)
    ANSFbits.ANSF5 = 0;         // Digital Pin;
    TRISFbits.TRISF5 = 0;       // Output;
    PORTFbits.RF5 = 1;          // Initial value (SCK is active low -> initial high)
    
    /*---------- 2. PPS configuration ----------*/
    
    // Defined in the PPS.h
    // Always follow the sequence: Unlock -> Set configuration -> Lock
    
    PPSUnLock;  // Unlock PPS
    iPPSOutput(OUT_PIN_PPS_RP14, OUT_FN_PPS_SS1OUT);    // Set FSYNC RB14 (RP14/AN14) as FSYNC (/SS)
    iPPSOutput(OUT_PIN_PPS_RP29, OUT_FN_PPS_SDO1);      // Set RB15 (RP29/AN15) as SDO
    iPPSInput(IN_FN_PPS_SDI1, IN_PIN_PPS_RP10);         // Set RF4 (RP10/AN11) as SDI
    iPPSOutput(OUT_PIN_PPS_RP17, OUT_FN_PPS_SCK1OUT);   // Set RF5 (RP17/AN10) as SCK
    PPSLock;    // Lock PPS   
    
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

unsigned char WriteSPI(unsigned char data_out)
{
    char dummy;
    
    dummy = SPI1BUF;        // Dummy read of the SPI1BUF register to clear the SPIRBF flag
    SPI1BUF = data_out;     // Write command to buffer
    while( !SPI1STATbits.SPIRBF )               // Wait for the data to be sent out
        ;
  
    return 1;
}

void Generator(long int n,int waveform)     // n -> output of wanted frequency
{
    unsigned char LSB_UP, LSB_LOW, MSB_UP, MSB_LOW;     // For frequency register 
    unsigned char RESET_UP,RESET_LOW, UNRESET_UP, UNRESET_LOW;  // for control bits
    long int MSB,LSB;
    
    /********************************************/
    /* For Control bits                         */ 
    /********************************************/
    if(waveform == 0 )     // For square wave
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
    if( n < 10)             // Frequency output = 10Hz..10MHz
		n = 10;
	
	if(n > 1000000)
		n = 1000000;
	
	n = (long int)(RATIO * n);          // FREQREG = (2^28 / FMCLK) * Fout = RATIO * n
    
    LSB = n % 16384;                    // 28/2=14, 2^14=16384 => get LSB(14bits)
	MSB = (n - LSB) / 16384 + 16384;    // Frequency - LSB -> right shift 14 bits 
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
    PORTBbits.RB14 = 0;     // Making FSYNC of AD9833 LOW before writing in
    //PORTGbits.RG9 = 0;
    WriteSPI(RESET_UP);     // Reset(D8); Control(D15,D14); both LSB and MSB FREQ writes consecutively(D13);
    WriteSPI(RESET_LOW);    // The internal clock is enabled(D7) ;the DAC has been put to sleep(D6);
                            // The output is to be a square wave (D5); the square wave frequency is not to be divided by two.
    
    // FREQ register LSB write in
    WriteSPI(LSB_UP);   //LSB
    WriteSPI(LSB_LOW);
    
    // FREQ register MSB write in
    WriteSPI(MSB_UP);   //MSB
    WriteSPI(MSB_LOW);
    
    // PHASE register write in
    WriteSPI(0b11000000);   //writing to the PHASE0 register (D15,D14,D13) (No need for phase shift -> all 0s)
    WriteSPI(0b00000000);   //and is writing a twelve bit value of zero (D11-D0)
    
    // Activate the output (disable reset)
    WriteSPI(UNRESET_UP);
    WriteSPI(UNRESET_LOW); 
    //PORTGbits.RG9 = 1;
    PORTBbits.RB14 = 1;  // Making FSYNC of AD9833 HIGH after writing in
    
}

void SquareWave_10K(void)
{
    //Control 16bits write in
    PORTBbits.RB14 = 0;  	//Making FSYNC of AD9833 LOW before writing in
    
    WriteSPI(0b00100001);   //Reset(D8); Control(D15,D14); both LSB and MSB FREQ writes consecutively(D13);
    WriteSPI(0b01101000);   // the internal clock is enabled(D7) ;the DAC has been put to sleep(D6);
                            // the output is to be a square wave (D5); the square wave frequency is not to be divided by two(D3).
   
  
    
    //FREQ register LSB write in
   
    WriteSPI(0b01011000);   //LSB
    WriteSPI(0b10010011);
   
    
    //FREQ register MSB write in
    
    WriteSPI(0b01000000);   //MSB
    WriteSPI(0b00010000);
    
    
    //PHASE register write in
    
    WriteSPI(0b11000000);   //writing to the PHASE0 register (D15,D14,D13) 
    WriteSPI(0b00000000);   //and is writing a twelve bit value of zero (D11-D0)
    
    
    //activate the output (disable reset)
    
    WriteSPI(0b00000000);
    WriteSPI(0b01101000);
    PORTBbits.RB14 = 1;
   
}

