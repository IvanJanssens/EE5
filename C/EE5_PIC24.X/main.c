#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config_EE5.h"
#include "ADC.h"
#include "multimeter_pic24.h"
#include "UART.h"
#include "connectionprotocol.h"

#define CLOCK_FREQ 20000000ULL

void initChip(void);
void counter(void);
void _ISR _ADC1Interrupt(void);
int C_A = 0;
int C_B = 0;
int C_M  = 0;
int count = 0;
char AD_done = 0;
unsigned int buffer_MM[10] = {0};
unsigned int buffer_A[1000] = {0};
unsigned int buffer_B[1000] = {0};

int main(void) {
    initChip();
//    init_ADC(A, B, M); //check here which channel you want to configure
//    init_MM();
    uart();
    while(1){
        if(!AD_done){
//            MM(buffer_A[C_A]);
            ///// LATD = (buffer_A[C_A-1]);
//            AD_done = ADC();  
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

void __attribute__((__interrupt__, auto_psv )) _ADC1Interrupt(void){
    
    LATBbits.LATB0 = 1;
    
    if (IFS0bits.AD1IF == 1) {
        //while(AD_done != 0) {
            IFS0bits.AD1IF = 0;
            if(ADL2STATbits.ADLIF) { //Multimeter
                ADL2STATbits.ADLIF = 0;
                buffer_MM[C_M] = ADRES2;
                C_M++;
                if(C_M >= 10) C_M = 0;
                AD_done--;
            }
            if(ADL0STATbits.ADLIF) { // VOUT_A
                ADL0STATbits.ADLIF = 0;
                C_A++;
                if(C_A >= 1000) C_A = 0;
                buffer_A[C_A] = ADRES0;
                AD_done--;
            }
            if(ADL1STATbits.ADLIF) { // Vout_B
                ADL1STATbits.ADLIF = 0;
                buffer_B[C_B] = ADRES1;
                C_B++;
                if(C_B >= 1000) C_B = 0;
                AD_done--;
            }
        //}
        //count++;
        //LATB = ADRES0*8; // & 0x0E00; //0000 1110 0000 0000
        //LATB = count;
    LATBbits.LATB0 = 0;
    } 
}

void counter(void) {
    int i = 0;
    int j = 0;
    unsigned int number = 16;
    LATB = 1;
    

    while (1) {
        for (i = 0; i < 5000; i++) {
            for (j = 0; j < 6024; j++) {
            }
            number++;
                if (number > 256) {
                    number = 1;
                }
                LATB = number;
        }
    }
}

