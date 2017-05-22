#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "DAC.h"
#include "connectionprotocol.h"

int dac_abits_mem, dac_bbits_mem;
float dac_a, dac_b;

void init_DAC(void) {
    // control DAC1 for OSC_A (10bit), hex code : 0x98FE, binary code : 0001 1000 1111 1110
    DAC1CONbits.DACEN = 0;
    DAC1CON = 0x18FE; // 

    DAC1DAT = 0x7440; // gives 1.5 at DAC_A output
    dac_abits_mem = 0x7440; // bin : 0111010001000000
    DAC1CONbits.DACEN = 1;
    // control DAC2 for OSC_B (10bit), hex code : 0x98FE, binary code : 0001 1000 1111 1110
    DAC2CONbits.DACEN = 0;
    DAC2CON = 0x18FE;

    DAC2DAT = 0x7440; // gives 1.5 at DAC_B output
    DAC2CONbits.DACEN = 1;
    dac_bbits_mem = 0x7440; // bin : 0111010001000000
}

void modify_ADAC(char u_d) {

    if (u_d){dac_abits_mem += 9*64 ; DAC1DAT = dac_abits_mem; } // 0000 0000 0000 1001
    
    else{  dac_abits_mem -= 9*64; DAC1DAT = dac_abits_mem;}
}

void modify_BDAC(char u_d) {

    if (u_d){dac_bbits_mem += 9*64; DAC2DAT = dac_bbits_mem; }
    else{  dac_bbits_mem -= 9*64; DAC2DAT = dac_bbits_mem;}
}

void OSC(void) {
    // switches between AC mode ( mode_x == 0 ) and DC mode (mode_x == 1)   
    DC_A = info.A.AC_DC;
    DC_B = info.B.AC_DC;
    DAC1DAT = info.A.offset*16;
    DAC2DAT = info.B.offset*16;
    
    if (info.A.gain == 4) { // /10 (1k Ohm)/ Y5 
        S0_A = 1;
        S1_A = 0;

    }
    else if (info.A.gain == 2) { // /5 (2k Ohm) / Y7
        S0_A = 1;
        S1_A = 1;
    }
    else if (info.A.gain == 1) { // /2 (5k Ohm) / Y6 
        S0_A = 0;
        S1_A = 1;
    }
    else if (info.A.gain == 0) { // /1 (10k Ohm) / Y4 
        S0_A = 0;
        S1_A = 0;
    }

    if (info.B.gain == 4) { // /10 (1k Ohm)/ Y5
        S0_B = 1;
        S1_B = 0;

    }
    else if (info.B.gain == 2) { // /5 (2k Ohm) / Y7
        S0_B = 1;
        S1_B = 1;
    }
    else if (info.B.gain == 1) { // /2 (5k Ohm) / Y6
        S0_B = 0;
        S1_B = 1;
    }
    else if (info.B.gain == 0) { // /1 (10k Ohm) / Y4
        S0_B = 0;
        S1_B = 0;
    }
}

float get_osc_input(float famp, float acq, int a_not_b) {
    if (a_not_b == 1) {
        dac_abits_mem = dac_abits_mem / (2 * 2 * 2 * 2 * 2 * 2);
        dac_a = (float) dac_abits_mem;
        return dac_a - (10.0f)*(((1.65f) -((1.00f) / famp)*((acq) - (1.65f))) - dac_a);
    } else {

        dac_bbits_mem = dac_bbits_mem / (2 * 2 * 2 * 2 * 2 * 2);
        dac_b = (float) dac_bbits_mem;
        return dac_b - (10.0f)*(((1.65f) -((1.00f) / famp)*((acq) - (1.65f))) - dac_b);
    }

}

char DAC_A(void){
    
    int dummy = ADRES0 & 0x0FFF;
    float out = (float) (       ( dummy   /     (4096.0f)  )*(3.3f)       )  ;
        
    if ( out <= (1.70f) && out >= (1.60f) ) return 1;
    else if (out > (1.70f) ) {modify_ADAC(1); return -1;}
    else {modify_ADAC(0); return -1;} }
    

char DAC_B(void){
    
    int dummy = ADRES1 & 0x0FFF;
    float out = (float) (       ( dummy   /     (4096.0f)  )*(3.3f)       );
    
    if ( out <= (1.70f) && out >= (1.60f) ) return 1;
    else if (out > (1.70f) ) {modify_BDAC(1); return -1;}
    else {modify_BDAC(0); return -1;} }
    

