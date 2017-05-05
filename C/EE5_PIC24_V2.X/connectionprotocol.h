/* 
 * File:   connectionprotocol.h
 * Author: Ivan
 *
 * Created on 4 mei 2017, 16:31
 */

#ifndef CONNECTIONPROTOCOL_H
#define	CONNECTIONPROTOCOL_H

void parseData(char data);

const unsigned char ERROR = 0b10101010;

union _moduleSelect {
    char allBits;
    struct {
        unsigned int oscilloscope    :1; //oscilloscope ON/OFF
        unsigned int multimeter    :1; //multimeter ON/OFF
        unsigned int functiongenerator    :1; //function generator ON/OFF
    };
} moduleSelect;

extern union _oscilloscopeParam {
    char allBits;
    struct {
        unsigned int samplingspeed    :3; //sampling speed of oscilloscope (fixed rates)
        unsigned int oscilloscope     :1; //Oscilloscope selection 1: A, 0: B
        unsigned int gainA            :2; //gain of oscilloscope A
        unsigned int gainB            :2; //gain of oscilloscope B
    };
} oscilloscopeParam;

extern union _functionGeneratorParam {
    char allBits;
    struct {
        unsigned int waveType   :4; //wave type select
        unsigned int frequency  :20; // frequency
    };
} functionGeneratorParam;

union _paramSelect {
    char allBits;
    struct {
        unsigned int incomingParam  : 1; //if param incoming = 1, if module incoming = 0
        unsigned int paramForMod : 1; //0 if param are for oscilloscope, 1 if param are for functiongenerator
        unsigned int numOfParam : 2; //number of param received
    };
} paramSelect;


#endif	/* CONNECTIONPROTOCOL_H */

