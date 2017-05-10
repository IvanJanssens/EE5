#ifndef DAC_H
#define	DAC_H

#include <xc.h>

#define S0_B LATBbits.LATB3
#define S1_B LATBbits.LATB4

#define S0_A LATBbits.LATB15
#define S1_A LATFbits.LATF4


void Init_DAC(void);

void DAC_write(int dac_a, int dac_b, int dac_a_bits, int dac_b_bits);

float get_osc_input(float famp, float acq, int a_not_b);

void dac_amp_select(int sel_a, int sel_b, int osc_a, int osc_b);



#endif	/* XC_HEADER_TEMPLATE_H */

