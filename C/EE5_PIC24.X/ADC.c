#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ADC.h"

char A, B, M;

void init_ADC() {
    
    ANSB = 0;
    ANSBbits.ANSVO_A = 1;
    ANSBbits.ANSVO_B = 1;
    ANSDbits.ANSVO_MM = 1;
    
    TRISBbits.TRISVO_A = 1;
    TRISBbits.TRISVO_B = 1;
    TRISDbits.TRISVO_MM = 1;
    
    ADCON1bits.ADON = 0;
    ADCON1 = 0x1041; // 0001 0000 0100 0001 checked
    ADCON2 = 0x0700; // 0000 0111 0000 0000 checked //indexed buffer
    ADCON3 = 0x0031; // 0000 0000 0000 0001 checked
    ADCON3bits.SLEN0 = A;
    ADCON3bits.SLEN1 = B;
    ADCON3bits.SLEN2 = M;
    
    int i;
    for(i = 20; i>0; i--) ADCON1bits.ADCAL = 1;
    ADCON1bits.ADON = 1;
    while(!ADSTATHbits.ADREADY);
    
    ADL0CONH = 0xA001; // 1010 0000 0000 0001
    ADL0CONL = 0x2200; // 0010 0010 0000 0000 : 1 sample registers
    ADL1CONH = 0xA001; // 1010 0000 0000 0001
    ADL1CONL = 0x2200; // 0010 0010 0000 0000 : 1 sample registers
    ADL2CONH = 0xA001; // 1010 0000 0000 0001
    ADL2CONL = 0x2200; // 0010 0010 0000 0000 : 1 sample registers
    
    ADTBL0 = 0x000E; // 0000 0000 0000 1110 ==>RB14: VOA
    ADTBL1 = 0x0005; // 0000 0000 0000 0101 ==>RB5: VOB
    ADTBL2 = 0x0019; // 0000 0000 0001 1001 ==>RD2: Multimeter
    
    ACCONH = 0;
    
   //for interrupts
    INTCON1bits.NSTDIS = 1; //no nesting interrupts
    IFS0= 0; //setting the flag on zero
    IPC3bits.AD1IP = 7; //Highest priority
    IEC0bits.AD1IE = 1; //enabling interrupts

    
    ADL0STAT = 0; //clearing the ADLIF: A/D Sample List Interrupt Event Flag bit
    ADSTATL = 0; //clearing all interrupt flag bits
}
char ADC(void) {
    char AD_done = 0;
    if(A) {
        ADL0CONLbits.SLEN = 1;
        AD_done++;
    } 
    if(B) {
        ADL1CONLbits.SLEN = 1;
        AD_done++;
    }
    if(M) {
        ADL2CONLbits.SLEN = 1;
        AD_done++;
    }
    return AD_done;
}

