#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ADC.h"
#include "multimeter_pic24.h"
#include "connectionprotocol.h"
#include "DAC.h"
#include "FIFO.h"

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

    ACCONH = 0;
    
   //for interrupts
    INTCON1bits.NSTDIS = 1; //no nesting interrupts
    IFS0= 0; //setting the flag on zero
    IPC3bits.AD1IP = 7; //Highest priority
    IEC0bits.AD1IE = 1; //enabling interrupts

    ADL0STAT = 0; //clearing the ADLIF: A/D Sample List Interrupt Event Flag bit
    ADSTATL = 0; //clearing all interrupt flag bits
}

void ADC(void){
    ADCON1bits.ADON = 0;
    
    ANSBbits.ANSVO_A = info.A.ON;
    ANSBbits.ANSVO_B = info.B.ON;
    ANSDbits.ANSVO_MM = info.MM.ON;
    
    TRISBbits.TRISVO_A = info.A.ON;
    TRISBbits.TRISVO_B = info.B.ON;
    TRISDbits.TRISVO_MM = info.MM.ON;
    
    ADCON3bits.SLEN0 = (info.A.ON | info.B.ON | info.MM.ON);
    
    if(info.A.ON && info.B.ON){
        ADL0CONL = 0x2201; // 0010 0010 0000 0001 : 2 sample registers: A and B
        ADTBL0 = 0x000E; // 0000 0000 0000 1110 ==>RB14: VOA
        ADTBL1 = 0x0005; // 0000 0000 0000 0101 ==>RB5: VOB
    }
    else if (info.A.ON || info.B.ON || info.MM.ON) {
        ADCON3bits.SLEN0 = 1;
        ADL0CONL = 0x2200; // 0010 0010 0000 0001 : 1 sample registers: A OR B OR MM
        if(info.A.ON) ADTBL0 = 0x000E; // 0000 0000 0000 1110 ==>RB14: VOA
        else if(info.B.ON) ADTBL0 = 0x0005; // 0000 0000 0000 0101 ==>RB5: VOB
        else if(info.MM.ON) ADTBL0 = 0x0019; // 0000 0000 0001 1001 ==>RD2: Multimeter
    }
    else {
        ADCON3bits.SLEN0 = 0;
    }
    
    int i;
    for(i = 20; i>0; i--) ADCON1bits.ADCAL = 1;
    ADCON1bits.ADON = 1;
    while(!ADSTATHbits.ADREADY);
}

void __attribute__((__interrupt__, auto_psv )) _ADC1Interrupt(void){
    if (IFS0bits.AD1IF == 1) {
        IFS0bits.AD1IF = 0;
        if(ADL0STATbits.ADLIF) {
            ADL0STATbits.ADLIF = 0;
            write_FIFO_tx(ADRES0);
            if(info.MM.ON){
                int var = ADRES0;
                int var1 = (0x0F80 & var)/128;
                int var2 = 0x007C & var;
                write_FIFO_tx(192 | var1);
                write_FIFO_tx(224 | var2);
                info.MM.value = var;
                info.MM.flag = 1;
            }
            if(info.A.ON){
                int var = ADRES0;
                int var1 = (0x0F80 & var)/128;
                int var2 = 0x007C & var;
                write_FIFO_tx(64 | (var1 & 0x1F));
                write_FIFO_tx(96 | (var2 & 0x1F));
            }
            if(info.B.ON){
                int var;
                if(info.A.ON) var = ADRES1;
                else var = ADRES0;
                int var1 = (0x0F80 & var)/128;
                int var2 = 0x007C & var;
                write_FIFO_tx(128 | (var1 & 0x1F));
                write_FIFO_tx(160 | (var2 & 0x1F));
            }
        }
    }
    AD_DONE = 1;
}

