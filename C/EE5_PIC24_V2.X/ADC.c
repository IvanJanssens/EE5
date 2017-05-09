#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ADC.h"
#include "multimeter_pic24.h"

int A, B, M;
unsigned int buffer_MM[10] = {0};
unsigned int buffer_A[1000] = {0};
unsigned int buffer_B[1000] = {0};
int C_A = 0;
int C_B = 0;
int C_M  = 0;
int AD_done;

void init_ADC(void) {
    ANSB = 0;
    
    ADCON1bits.ADON = 0;
    ADCON1 = 0x1041; // 0001 0000 0100 0001 checked
    ADCON2 = 0x0700; // 0000 0111 0000 0000 checked //indexed buffer
    ADCON3 = 0x0003; // 0000 0000 0001 0000 checked
    
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

void init_MM(int ADC_MM) {
    cases(0);
    M = ADC_MM;
    ANSDbits.ANSVO_MM = M;
    TRISDbits.TRISVO_MM = M;
    
    ADCON1bits.ADON = 0;
    
    ADCON3bits.SLEN2 = M;
    int i;
    for(i = 20; i>0; i--) ADCON1bits.ADCAL = 1;
    ADCON1bits.ADON = 1;
    while(!ADSTATHbits.ADREADY);
    
}

void init_OSC_A(int ADC_A) {
    A = ADC_A;
    ANSBbits.ANSVO_A = A;
    TRISBbits.TRISVO_A = A;
    
    ADCON1bits.ADON = 0;
    ADCON3bits.SLEN0 = A;
    
    int i;
    for(i = 20; i>0; i--) ADCON1bits.ADCAL = 1;
    ADCON1bits.ADON = 1;
    while(!ADSTATHbits.ADREADY);
    
}

void init_OSC_B(int ADC_B) {
    B = ADC_B;
    ANSBbits.ANSVO_B = B;
    TRISBbits.TRISVO_B = B;
    
    ADCON1bits.ADON = 0;
    ADCON3bits.SLEN1 = B;
    
    int i;
    for(i = 20; i>0; i--) ADCON1bits.ADCAL = 1;
    ADCON1bits.ADON = 1;
    while(!ADSTATHbits.ADREADY);
    
}

int ADC(void) {
    int done = 0;
    if(A) {
        ADL0CONLbits.SLEN = 1;
        done++;
    } 
    if(B) {
        ADL1CONLbits.SLEN = 1;
        done++;
    }
    if(M) {
        ADL2CONLbits.SLEN = 1;
        done++;
    }
    return done;
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

