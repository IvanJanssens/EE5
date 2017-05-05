/* 
 * File:   connectionprotocol.h
 * Author: Ivan
 *
 * Created on 4 mei 2017, 16:31
 */

#ifndef CONNECTIONPROTOCOL_H
#define	CONNECTIONPROTOCOL_H

void parseData(unsigned int data);

extern union _oscilloscopeParam{
    char allBits;
    struct {
        unsigned int samplingspeed    :3; //sampling speed of oscilloscope (fixed rates)
        unsigned int oscilloscope     :1; //Oscilloscope selection 1: A, 0: B
        unsigned int gainA            :2; //gain of oscilloscope A
        unsigned int gainB            :2; //gain of oscilloscope B
        unsigned int ACDCA            :1; // AD/DC mode oscilloscope A
        unsigned int ACDCB            :1; // AC/DC mode oscilloscope B
    };
} oscilloscopeParam;


extern union _functionGeneratorParam{
    char allBits;
    struct {
        unsigned int waveType   :4; //wave type select
        unsigned long int frequency  :20; // frequency
    };
} functionGeneratorParam;



#endif	/* CONNECTIONPROTOCOL_H */

