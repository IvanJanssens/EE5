#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config_EE5.h"
#include "ADC.h"

#define CLOCK_FREQ 20000000ULL

void initChip(void);
void counter(void);
void _ISR _ADC1Interrupt(void);
int count = 0;

int main(void) {
    initChip();
    init_ADC();
    
    while(1){
        ADC();
        //counter();
        //PORTB = 0xFFFF;
        //PORTBbits.RB15 = 0;
        //PORTBbits.RB13 = 1;
    }
    
    return 0;
}

void initChip(void) {
    OSCCONbits.COSC = 2; // Primary Oscillator (XT, HS? EC)
    OSCCONbits.NOSC = 2; // HS: High Frequency
    CLKDIVbits.DOZEN = 0;  //CPU-to-peripheral clock ratio
    CLKDIVbits.PLLEN = 1;   //always active PLL
    
    RPOR5bits.RP10R = 18; // RP10 is configured as output of output compare 1
    OC1CON1 = 0;
    OC1CON1bits.OCTSEL = 7;
    
    
}

void _ISR _ADC1Interrupt(void){
    
    //__delay_ms(200);
    PORTBbits.RB0 = 0;
    if (IFS0bits.AD1IF == 1) {
        IFS0bits.AD1IF = 0;
        TRISBbits.TRISB14 = 1;
        count++;
        PORTB = ADRES0*16; // & 0x0E00; //0000 1110 0000 0000
        //PORTB = count;
        TRISBbits.TRISB0 = 0;
        PORTBbits.RB0 = 1;
    } 
    else {
        TRISDbits.TRISD9 = 0;
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
            number = number++;
                if (number > 256) {
                    number = 1;
                }
                PORTB = number;
        }
    }
}

