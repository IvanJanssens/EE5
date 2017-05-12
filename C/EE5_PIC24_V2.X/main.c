#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config_EE5.h"
#include "ADC.h"
#include "DAC.h"
#include "UART.h"
#include "multimeter_pic24.h"
#include "connectionprotocol.h"
#include "FIFO.h"

#define CLOCK_FREQ 20000000ULL

void init_Chip(void);
void init_ALL(void);
int AD_count = 0;

int main(void) {
    init_Chip();
    init_ALL();
    
    info.allbits = 0;
    while(1){
        if (get_count() != 0) read_FIFO();
        if(AD_count == 0){
            //MM(buffer_A[C_A]);
            ///// LATD = (buffer_A[C_A-1]);
            AD_count = ADC();  
        }
    }
    return 0;
}

void init_Chip(void) {
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
}

void init_ALL() {
    init_ADC();
    init_MM();
    init_A();
    init_B();
    init_FIFO();
    set_UART();
}
