#ifndef __FUNCTION_GENERATOR__H
#define __FUNCTION_GENERATOR__H


/***********************
 * RB15(RP29/AN15) = SDO  
 * RF4 (RP10/AN11) = SDI
 * RF5 (RP17/AN10) = SDK
 * RB14(RP14/AN14) = FSYNC
 * ********************/

//------------------------------------------------------------------------------
//Public defintions
//------------------------------------------------------------------------------
#define RATIO 13.4217728 // Assume the input frequency is 20MHz, RATIO = 2^28 / FMCLK 

//------------------------------------------------------------------------------
//Public prototypes
//------------------------------------------------------------------------------

void initFG(void);                    // Initialize the Function Generator configuration
unsigned char WriteSPI(unsigned char);  // Write SPI function
void Generator(long int, int);          // Generate the output
void SquareWave_10K(void);              // Testing function: generate the 10KHz square wave

#endif //FUNCTION_GENERATOR__H