/* 
 * File:   main.c
 * Author: Jerom Van Derbeken
 *
 * Created on March 3, 2017, 2:57 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config_EE5.h"
#include "ADC.h"


void initChip(void);
void counter(void);

int main(void) {
    initChip();
    //initADC();
    //ADC();
    while(1){
        counter();
        PORTB = 0xFFFF;
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

