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
    int i;
    unsigned char buffer[5] = {1, 5, 70, 90, 3};
    info.allbits = 0;
    while(1){

        for(i = 0; i < 5; i++){
            write_FIFO_tx(buffer[i]);
        }
        if (get_count_rx() != 0) read_FIFO_rx();
        if(get_count_tx() != 0) send_FIFO_tx();
//        if(AD_count == 0){
//            //MM(buffer_A[C_A]);
//            ///// LATD = (buffer_A[C_A-1]);
//            AD_count = ADC();  
//        }
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
