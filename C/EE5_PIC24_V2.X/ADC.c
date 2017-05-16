#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ADC.h"
#include "multimeter_pic24.h"
#include "connectionprotocol.h"
#include "DAC.h"
#include "FIFO.h"

int A, B, M;
unsigned int buffer_MM[10] = {0};
unsigned int buffer_A[1000] = {0};
unsigned int buffer_B[1000] = {0};
int C_A = 0;
int C_B = 0;
int C_M  = 0;
int AD_DONE;

void init_ADC(void) {
    ANSB = 0;
    
    ADCON1bits.ADON = 0;
    ADCON1 = 0x1041; // 0001 0000 0100 0001 checked
    ADCON2 = 0x0700; // 0000 0111 0000 0000 checked //indexed buffer
    ADCON3 = 0x0003; // 0000 0000 0000 0011 checked
    
    int i;
    for(i = 20; i>0; i--) ADCON1bits.ADCAL = 1;
    ADCON1bits.ADON = 1;
    while(!ADSTATHbits.ADREADY);
    
    ADL0CONH = 0xA001; // 1010 0000 0000 0001
    ADL0CONL = 0x2200; // 0010 0010 0000 0000 : 1 sample registers //MM
    ADL1CONH = 0xA001; // 1010 0000 0000 0001
    ADL1CONL = 0x2201; // 0010 0010 0000 0001 : 2 sample registers //A en B
    
    ADTBL0 = 0x0019; // 0000 0000 0001 1001 ==>RD2: Multimeter
    ADTBL1 = 0x000E; // 0000 0000 0000 1110 ==>RB14: VOA
    ADTBL2 = 0x0005; // 0000 0000 0000 0101 ==>RB5: VOB
    
    ACCONH = 0;
    
   //for interrupts
    INTCON1bits.NSTDIS = 1; //no nesting interrupts
    IFS0= 0; //setting the flag on zero
    IPC3bits.AD1IP = 7; //Highest priority
    IEC0bits.AD1IE = 1; //enabling interrupts

    ADL0STAT = 0; //clearing the ADLIF: A/D Sample List Interrupt Event Flag bit
    ADSTATL = 0; //clearing all interrupt flag bits
}

void init_MM(void) {
    cases(0);
    M = info.MM.ON;
    ANSDbits.ANSVO_MM = M;
    TRISDbits.TRISVO_MM = M;
    
    ADCON1bits.ADON = 0;
    
    ADCON3bits.SLEN2 = M;
    int i;
    for(i = 20; i>0; i--) ADCON1bits.ADCAL = 1;
    ADCON1bits.ADON = 1;
    while(!ADSTATHbits.ADREADY);
}

void init_A(void) {
    A = info.A.ON;
    dac_gain_select_A();
    
    ANSBbits.ANSVO_A = A;
    TRISBbits.TRISVO_A = A;
    
    ADCON1bits.ADON = 0;
    ADCON3bits.SLEN0 = A;
    
    int i;
    for(i = 20; i>0; i--) ADCON1bits.ADCAL = 1;
    ADCON1bits.ADON = 1;
    while(!ADSTATHbits.ADREADY);
}

void init_B(void) {
    B = info.B.ON;
    dac_gain_select_B();
    
    ANSBbits.ANSVO_B = B;
    TRISBbits.TRISVO_B = B;
    
    ADCON1bits.ADON = 0;
    ADCON3bits.SLEN1 = B;
    
    int i;
    for(i = 20; i>0; i--) ADCON1bits.ADCAL = 1;
    ADCON1bits.ADON = 1;
    while(!ADSTATHbits.ADREADY);
}

void ADC(void) {
    AD_DONE = 0;
    if(M) {
        ADL0CONLbits.SLEN = 1;
    }
    if(A || B) {
        ADL1CONLbits.SLEN = 1;
    } 
}

void __attribute__((__interrupt__, auto_psv )) _ADC1Interrupt(void){
    
    LATBbits.LATB0 = 1;
    
    if (IFS0bits.AD1IF == 1) {
        IFS0bits.AD1IF = 0;
        if(ADL0STATbits.ADLIF) { //Multimeter
            ADL0STATbits.ADLIF = 0;
            int var = ADRES0;
            int var1 = (0x0F80 & var)/128;
            int var2 = 0x007C & var;
            write_FIFO_tx(192 | var1);
            write_FIFO_tx(224 | var2);
        }
        if(ADL1STATbits.ADLIF) { // VOUT_A
            ADL1STATbits.ADLIF = 0;
            if(info.A.ON){
                int var = ADRES1;
                int var1 = (0x0F80 & var)/128;
                int var2 = 0x007C & var;
                write_FIFO_tx(64 | var1);
                write_FIFO_tx(96 | var2);
            }
            if(info.B.ON){
                int var = ADRES2;
                int var1 = (0x0F80 & var)/128;
                int var2 = 0x007C & var;
                write_FIFO_tx(128 | var1);
                write_FIFO_tx(160 | var2);
            }
        }
    }
    AD_DONE = 1;
}

