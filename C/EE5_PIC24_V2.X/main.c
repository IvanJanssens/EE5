#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config_EE5.h"
#include "ADC.h"
#include "DAC.h"
#include "multimeter_pic24.h"
#include "UART.h"
#include "connectionprotocol.h"

#define CLOCK_FREQ 20000000ULL

void initChip(void);
int AD_count = 0;

int main(void) {
    initChip();
//    init_ADC(A, B, M); //check here which channel you want to configure
//    init_MM();
    uart();
    while(1){
        if(AD_count == 0){
            //MM(buffer_A[C_A]);
            ///// LATD = (buffer_A[C_A-1]);
            AD_count = ADC();  
        }
    }
    return 0;
}

void initChip(void) {
    TRISB = 0x40E0; // 0100 0000 0000 1110 0000
    TRISCbits.TRISC12 = 1;
    TRISCbits.TRISC15 = 0;
    TRISD = 0x0004; //0000 0000 0000 0100
    
    //oscillator
    OSCCONbits.COSC = 2; // Primary Oscillator (XT, HS? EC)
    OSCCONbits.NOSC = 2; // HS: High Frequency
    CLKDIVbits.DOZEN = 0;  //CPU-to-peripheral clock ratio
    CLKDIVbits.PLLEN = 1;   //always active PLL
    
    RPOR5bits.RP10R = 18; // RP10 is configured as output of output compare 1
    OC1CON1 = 0;
    OC1CON1bits.OCTSEL = 7;
    
    //which module you want to use
    A = 1;
    B = 0;
    M = 0; //M is not right
    
}

