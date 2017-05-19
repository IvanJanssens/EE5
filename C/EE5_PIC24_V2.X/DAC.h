#ifndef DAC_H
#define	DAC_H

#include <xc.h>

#define S0_B LATBbits.LATB3
#define S1_B LATBbits.LATB4

#define S0_A LATBbits.LATB15
#define S1_A LATFbits.LATF4

#define DC_A LATBbits.LATB12
#define DC_B LATBbits.LATB2 


void init_DAC(void);

void OSC(void);

float get_osc_input(float famp, float acq, int a_not_b);


#endif	/* DAC_H */

