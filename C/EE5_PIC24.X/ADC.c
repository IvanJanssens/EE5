#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ADC.h"

void init_ADC(char MM, char osc_A, char osc_B) {
    ANSB = 0;
    TRISB = 0;
    TRISD = 0;
    ANSBbits.ANSVO_B = 1;
    ANSBbits.ANSVO_A = 1;
    ANSDbits.ANSVO_MM = 1;
    TRISBbits.TRISVO_B = 1;
    TRISBbits.TRISVO_A = 1;
    TRISDbits.TRISVO_MM = 1;
    
    ADCON1bits.ADON = 0;
    ADCON1 = 0x1041; // 0001 0000 0100 0001 checked
    ADCON2 = 0x0300; // 0000 0011 0000 0000 checked
    ADCON3 = 0x0001; // 0000 0000 0001 0000 checked
    int i;
    for(i = 20; i>0; i--) {
        ADCON1bits.ADCAL = 1;
    }
    ADCON1bits.ADON = 1;
    while(!ADSTATHbits.ADREADY);
    
    ADL0CONH = 0xA001; // 1010 0000 0000 0001
    ADL0CONL = 0x2202; // 0010 0010 0000 0010 : 3 sample registers
    ADTBL0 = 0x0019; // 0000 0000 0001 1001 ==>RD2: Multimeter
    ADTBL1 = 0x000E; // 0000 0000 0000 1110 ==>RB14: VOA
    ADTBL2 = 0x0005; // 0000 0000 0000 0101 ==>RB5: VOB
    ACCONH = 0;
    
   //for interrupts
    INTCON1bits.NSTDIS = 1; //no nesting interrupts
    IFS0= 0; //setting the flag on zero
    PORTD = 0;
    IPC3bits.AD1IP = 7; //Highest priority
    IEC0bits.AD1IE = 1; //enabling interrupts

    
    ADL0STAT = 0; //clearing the ADLIF: A/D Sample List Interrupt Event Flag bit
    ADSTATL = 0; //clearing all interrupt flag bits
}
void ADC() {
    ADL0CONLbits.SLEN = 1;
}

