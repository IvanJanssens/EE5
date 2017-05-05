
/***********************
 * RB15(RP29/AN15) = SDO  
 * RF4 (RP10/AN11) = SDI
 * RF5 (RP17/AN10) = SDK
 * RB14(RP14/AN14) = FSYNC
 * ********************/

// CONFIG4
#pragma config DSWDTPS = DSWDTPS1F      // Deep Sleep Watchdog Timer Postscale Select bits (1:68719476736 (25.7 Days))
#pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select (DSWDT uses LPRC as reference clock)
#pragma config DSBOREN = ON             // Deep Sleep BOR Enable bit (DSBOR Enabled)
#pragma config DSWDTEN = ON             // Deep Sleep Watchdog Timer Enable (DSWDT Enabled)
#pragma config DSSWEN = ON              // DSEN Bit Enable (Deep Sleep is controlled by the register bit DSEN)
#pragma config RTCBAT = ON              // RTC Battery Operation Enable (RTC operation is continued through VBAT)
#pragma config PLLDIV = DIS             // PLL Input Prescaler Select bits (PLL is disabled)
#pragma config I2C2SEL = PRI            // Alternate I2C2 Location Select bit (I2C2 is multiplexed to SDA2/RA3 and SCL2/RA2 )
#pragma config IOL1WAY = ON             // PPS IOLOCK Set Only Once Enable bit (Once set, the IOLOCK bit cannot be cleared)

// CONFIG3
#pragma config WPFP = WPFP127           // Write Protection Flash Page Segment Boundary (Page 127 (0x1FC00))
#pragma config SOSCSEL = ON             // SOSC Selection bits (SOSC circuit selected)
#pragma config WDTWIN = PS25_0          // Window Mode Watchdog Timer Window Width Select (Watch Dog Timer Window Width is 25 percent)
#pragma config BOREN = ON               // Brown-out Reset Enable (Brown-out Reset Enable)
#pragma config WPDIS = WPDIS            // Segment Write Protection Disable (Disabled)
#pragma config WPCFG = WPCFGDIS         // Write Protect Configuration Page Select (Disabled)
#pragma config WPEND = WPENDMEM         // Segment Write Protection End Page Select (Write Protect from WPFP to the last page of memory)

// CONFIG2
#pragma config POSCMD = HS              // Primary Oscillator Select (HS Oscillator Enabled)
#pragma config WDTCLK = LPRC            // WDT Clock Source Select bits (WDT uses LPRC)
#pragma config OSCIOFCN = OFF           // OSCO Pin Configuration (OSCO/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Fail-Safe Clock Monitor Configuration bits (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Initial Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config ALTADREF = AVREF_RA      // External 12-Bit A/D Reference Location Select bit (AVREF+/AVREF- are mapped to RA9/RA10)
#pragma config ALTCVREF = CVREF_RA      // External Comparator Reference Location Select bit (CVREF+/CVREF- are mapped to RA9/RA10)
#pragma config WDTCMX = WDTCLK          // WDT Clock Source Select bits (WDT clock source is determined by the WDTCLK Configuration bits)
#pragma config IESO = ON                // Internal External Switchover (Enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler Select (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler Ratio Select (1:128)
#pragma config WINDIS = OFF             // Windowed WDT Disable (Standard Watchdog Timer)
#pragma config FWDTEN = WDT_HW          // Watchdog Timer Enable (WDT enabled in hardware)
#pragma config ICS = PGx1               // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC1/PGED1)
#pragma config LPCFG = OFF              // Low power regulator control (Disabled - regardless of RETEN)
#pragma config GWRP = OFF               // General Segment Write Protect (Disabled)
#pragma config GCP = OFF                // General Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <PPS.h>                        // Include the header for PPS function     


#define RATIO 13.4217728                // Assume the input frequency is 20MHz, RATIO = 2^28 / FMCLK 

void initChip(void);                    // Initialize the chip configuration
unsigned char WriteSPI(unsigned char);  // Write SPI function
void Generator(long int, int);          // Generate the output
void SquareWave_10K(void);              // Testing function: generate the 10KHz square wave

int main(void) {
    initChip();

    Generator(10000,1);                 // Send the command to AD9833
    //SquareWave_10K();
    
    while(1)
    {
        Nop();                          // Do nothing in the loop

    }
}

void initChip(void)
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

