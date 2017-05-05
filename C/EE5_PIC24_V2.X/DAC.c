#include <xc.h>
#include "DAC.h"

int dac_abits_mem, dac_bbits_mem;
//float dac_a, dac_b;

void initDAC(void) {
    // control DAC1 for OSC_A (10bit), hex code : 0x98FE, binary code : 1 0 011 00 0 1 11111 10
    DAC1CON = 0x98FE; // 

    DAC1DAT = 0x7440; // gives 1.5 at DAC_A output
    dac_abits_mem = 0x7440; // bin : 0111010001000000

    // control DAC2 for OSC_B (10bit), hex code : 0x98FE, binary code : 1 0 011 00 0 1 11111 10
    DAC2CON = 0x98FE;

    DAC2DAT = 0x7440; // gives 1.5 at DAC_B output
    dac_bbits_mem = 0x7440; // bin : 0111010001000000
}

void DAC_write(bit dac_a, bit dac_b, int dac_aBits, int dac_bBits) {
    if (dac_a == 1) {
        DAC1DAT = dac_aBits;
        dac_abits_mem = dac_aBits;
    }
    if (dac_b == 1) {
        DAC2DAT = dac_bBits;
        dac_bbits_mem = dac_bBits;
    }
}

float get_osc_input(float famp, float acq, bit a_not_b) {
    // if a_not_b is one, the fucntion will calculate the input of osc_a, else of osc_b
    
    // first_step = (1.65f) -((1.00f) / famp)*((acq) - (1.65f));
    // input = DAC - (10.0f)*(first_step - DAC)
    
    
    if (a_not_b == 1) {

        dac_abits_mem = dac_abits_mem / (2 * 2 * 2 * 2 * 2 * 2);
        dac_a = (float) = dac_abits_mem;
        return dac_a - (10.0f)*(((1.65f) -((1.00f) / famp)*((acq) - (1.65f))) - dac_a);
    } else {

        dac_bbits_mem = dac_bbits_mem / (2 * 2 * 2 * 2 * 2 * 2);
        dac_b = (float) = dac_bbits_mem;
        return dac_b - (10.0f)*(((1.65f) -((1.00f) / famp)*((acq) - (1.65f))) - dac_b);
    }

}

void dac_amp_select(int sel_a, int sel_b, bit osc_a, int osc_b) {
    if (osc_a == 1) { // OSC A
        if (sel_a == 1) { // 1 maal versterking (1k Ohm)/ Y5
            S0_A = 1;
            S1_A = 0;

        }

        if (sel_a == 2) { // 2 maal versterking (2k Ohm) / Y6
            S0_A = 1;
            S1_A = 1;
        }

        if (sel_a == 3) { // 5 maal versterking (5k Ohm) / Y6
            S0_A = 0;
            S1_A = 1;
        }

        if (sel_a == 4) { // 10 maal versterking (10k Ohm) / Y4
            S0_A = 0;
            S1_A = 0;
        }

    }
    if (osc_b == 1) { // OSC B

        if (sel_b == 1) { // 1 maal versterking (1k Ohm)/ Y5
            S0_B = 1;
            S1_B = 0;

        }

        if (sel_b == 2) { // 2 maal versterking (2k Ohm) / Y6
            S0_B = 1;
            S1_B = 1;
        }

        if (sel_b == 3) { // 5 maal versterking (5k Ohm) / Y6
            S0_B = 0;
            S1_B = 1;
        }

        if (sel_b == 4) { // 10 maal versterking (10k Ohm) / Y4
            S0_B = 0;
            S1_B = 0;
        }

    }
}
