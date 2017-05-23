#include <xc.h>
#include "DAC.h"
#include "connectionprotocol.h"
#include "multimeter_pic24.h"

int dac_a, dac_b;

void init_DAC(void) {
    // control DAC1 for OSC_A (10bit), hex code : 0x98FE, binary code : 0001 1000 1111 1110
    DAC1CONbits.DACEN = 0;
    DAC1CON = 0x18FE; // 

    DAC1DAT = 0x7440; // gives 1.5 at DAC_A output
    dac_a = 0x7440; // bin : 0111010001000000
    DAC1CONbits.DACEN = 1;
    // control DAC2 for OSC_B (10bit), hex code : 0x98FE, binary code : 0001 1000 1111 1110
    DAC2CONbits.DACEN = 0;
    DAC2CON = 0x18FE;

    DAC2DAT = 0x7440; // gives 1.5 at DAC_B output
    DAC2CONbits.DACEN = 1;
    dac_b = 0x7440; // bin : 0111010001 000000
}

char DAC_A(void){
    
    int dummy = ADRES0 & 0x0FFF;
    float out = (float) dummy*LSB;
        
    if ( out <= (1.70f) && out >= (1.60f) ) return 0;
    else if (out > (1.70f) ) dac_a += 9*64;
    else dac_a -= 9*64; 
    DAC1DAT = dac_a;
    info.A.offset = (dac_a/64);
    return -1;
}
    
char DAC_B(void){
    
    int dummy = ADRES1 & 0x0FFF;
    float out = (float)dummy*LSB;
    
    if ( out <= (1.70f) && out >= (1.60f) ) return 0;
    else if (out > (1.70f) ) dac_b += 9*64;
    else dac_b -= 9*64;
    DAC2DAT = dac_b;
    info.B.offset = (dac_b/64);
    return -1;
}

void OSC(void) {
    // switches between AC mode ( mode_x == 0 ) and DC mode (mode_x == 1)   
    DC_A = info.A.AC_DC;
    DC_B = info.B.AC_DC;
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
    

