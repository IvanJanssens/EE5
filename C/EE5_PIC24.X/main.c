#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config_EE5.h"
#include "ADC.h"
#include "multimeter_pic24.h"

#define CLOCK_FREQ 20000000ULL

void initChip(void);
void counter(void);
void _ISR _ADC1Interrupt(void);
int count = 0;
char AD_done = 0;
unsigned int buffer_MM[10] = {0};
unsigned int buffer_A[1000] = {0};
unsigned int buffer_B[1000] = {0};

int main(void) {
    initChip();
    init_ADC(1, 1, 1);
    init_MM();
    
    while(1){
        if(ADSTATLbits.SLOV == 1) return 0;
        if(AD_done){
            AD_done = 0;
            buffer_A[count] = ADRES0;
            buffer_B[count] = ADRES0;
            count++;
            if(count == 1000) count = 0;
            MM(ADRES0);
            ADC();  
        }
    }
    
    return 0;
}

void initChip(void) {
    //oscillator
    OSCCONbits.COSC = 2; // Primary Oscillator (XT, HS? EC)
    OSCCONbits.NOSC = 2; // HS: High Frequency
    CLKDIVbits.DOZEN = 0;  //CPU-to-peripheral clock ratio
    CLKDIVbits.PLLEN = 1;   //always active PLL
    
    RPOR5bits.RP10R = 18; // RP10 is configured as output of output compare 1
    OC1CON1 = 0;
    OC1CON1bits.OCTSEL = 7;
    
    TRISBbits.TRISB14 = 1;
    TRISBbits.TRISB0 = 0;
    TRISDbits.TRISD9 = 0;
    
}

void _ISR _ADC1Interrupt(void){
    
    PORTBbits.RB0 = 0;
    
    if (IFS0bits.AD1IF == 1) {
        IFS0bits.AD1IF = 0;
        count++;
        //PORTB = ADRES0*16; // & 0x0E00; //0000 1110 0000 0000
        PORTB = count;
        PORTBbits.RB0 = 1;
    } 
    else {
        PORTDbits.RD9 = 1;
    }
}

void counter(void) {
    int i = 0;
    int j = 0;
    unsigned int number = 16;
    PORTB = 1;
    

    while (1) {
        for (i = 0; i < 5000; i++) {
            for (j = 0; j < 6024; j++) {
            }
            number++;
                if (number > 256) {
                    number = 1;
                }
                PORTB = number;
        }
    }
}

