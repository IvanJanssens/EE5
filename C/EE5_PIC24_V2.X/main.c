#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <PPS.h>
#include "config_EE5.h"
#include "ADC.h"
#include "DAC.h"
#include "UART.h"
#include "multimeter_pic24.h"
#include "connectionprotocol.h"
#include "FIFO.h"
#include "FunctionGenerator.h"

#define CLOCK_FREQ 20000000ULL

void init_Chip(void);
void init_ALL(void);
int AD_count = 0;
int count = 0;



int main(void) {
    init_Chip();
    init_ALL();
    SquareWave_10K();
    info.allbits = 0;
    while(1){
        if (get_count_rx() != 0) {
            read_FIFO_rx();
            init_FIFO();
            ADL0CONLbits.SLEN = 1;
            count = 0;
        }
        if(get_count_tx() != 0) send_FIFO_tx();
        if(AD_DONE && count < 40) {
            if(info.A.ON || info.B.ON){
                AD_DONE = 0;
                ADL0CONLbits.SLEN = 1;
                count++;
            }
            else if(info.MM.ON && info.MM.flag){
                info.MM.flag = 0;
                MM(info.MM.value);
                AD_DONE = 0;
                ADL0CONLbits.SLEN = 1;
                count++;
            }
        }
    }
    return 0;
}

void init_Chip(void) {
    TRISB = 0x60E0; // 0110 0000 0000 1110 0000
    TRISCbits.TRISC12 = 1;
    TRISCbits.TRISC15 = 0;
    TRISD = 0x0004; //0000 0000 0000 0100
    
    TRISBbits.TRISB13 = 1;
    TRISGbits.TRISG9 = 1;    
    ANSBbits.ANSB13 = 1;
    ANSGbits.ANSG9 = 1;
    
    //oscillator
    OSCCONbits.COSC = 2; // Primary Oscillator (XT, HS? EC)
    OSCCONbits.NOSC = 2; // HS: High Frequency
    CLKDIVbits.DOZEN = 0;  //CPU-to-peripheral clock ratio
    CLKDIVbits.PLLEN = 1;   //always active PLL
    
    RPOR5bits.RP10R = 18; // RP10 is configured as output of output compare 1
    OC1CON1 = 0;
    OC1CON1bits.OCTSEL = 7;
    
    // PPS cannot be unlock twice;
    // Define all PPS here 
    PPSUnLock;
    
    // Part 1. UART
    iPPSInput(IN_FN_PPS_U2RX,IN_PIN_PPS_RP16);         // Assign UART2RX To Pin RP16 (RF3)
    iPPSOutput(OUT_PIN_PPS_RP17,OUT_FN_PPS_U2TX);      // Assign UART2TX To Pin RP17 (RF5)  
    
    // Part 2. SPI
    iPPSOutput(OUT_PIN_PPS_RP2, OUT_FN_PPS_SS1OUT);    // Set FSYNC RD8 (RP2) as FSYNC (/SS)
    iPPSOutput(RPOR1bits.RP3R, OUT_FN_PPS_SDO1);      // Set RD10 (RP3) as SDO
    iPPSInput(IN_FN_PPS_SDI1, IN_PIN_PPS_RP1);         // Set RB1 (RP1) as SDI
    iPPSOutput(OUT_PIN_PPS_RP4, OUT_FN_PPS_SCK1OUT);   // Set RD9 (RP4) as SCK
    
    PPSLock;
}

void init_ALL() {
    init_ADC();
    ADC();
    init_FIFO();
    set_UART();
    init_DAC();
    init_FG();
}
