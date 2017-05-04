/* 
 * File:   connectionprotocol.h
 * Author: Ivan
 *
 * Created on 4 mei 2017, 16:31
 */

#ifndef CONNECTIONPROTOCOL_H
#define	CONNECTIONPROTOCOL_H

void parseData(char data);

static char moduleSelect[8];
    /*
     * bit 7: Oscilloscope ON/OFF;
     * bit 6: Multimeter ON/OFF;
     * bit 5: Function generator ON/OFF;
     * bit 4 - 0: Free bits
     */
static char oscilloscopeParam[8];
    /*
     * bit 7 - 5: sampling speed of oscilloscope (fixed rates)
     * bit 4 - 0: Free bits
     */

static char functionGeneratorParam[24];
    /*
     * bit 23 - 20: Wave type select
     * bit 16 - 0: Frequency
     */


#endif	/* CONNECTIONPROTOCOL_H */

