#ifndef DAC_H
#define	DAC_H


#include <xc.h>

#define S0_B LATBbits.LATB3
#define S1_B LATBbits.LATB4

#define S0_A LATBbits.LATB15
#define S1_A LATFbits.LATF4




void initDAC(void);

void DAC_write(bit dac_a, bit dac_b, int dac_aBits, int dac_bBits);

float get_osc_input(float famp, float acq, bit a_not_b);

void dac_amp_select(int sel_a, int sel_b, bit osc_a, int osc_b);



#endif	/* XC_HEADER_TEMPLATE_H */

