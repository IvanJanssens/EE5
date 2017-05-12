#ifndef DAC_H
#define	DAC_H

#include <xc.h>

#define S0_B LATBbits.LATB3
#define S1_B LATBbits.LATB4

#define S0_A LATBbits.LATB15
#define S1_A LATFbits.LATF4

#define DC_A LATBbits.LATB12
#define DC_B LATBbits.LATB2 


void Init_DAC(void);

void AC_DC_mode(void);

void DAC_write(int dac_a, int dac_b, int dac_aBits, int dac_bBits);

float get_osc_input(float famp, float acq, int a_not_b);

void dac_gain_select_B(void);
void dac_gain_select_A(void);


#endif	/* DAC_H */

