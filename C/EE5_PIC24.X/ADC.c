#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


void init_ADC(void) {
    TRISB = 0;
    TRISBbits.TRISB0 = 1;
    ADCON1bits.ADON = 0;
    ADCON1 = 0x1001; // 0001 0000 0000 0001
    ADCON2 = 0x0300; // 0000 0011 0000 0000
    ADCON3 = 0x0001; // 0000 0000 0001 0000
    ADCON1bits.ADON = 1;
    while(!ADSTATHbits.ADREADY);
}
void ADC() {
    
}

