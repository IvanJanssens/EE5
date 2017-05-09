#ifndef CONNECTIONPROTOCOL_H
#define	CONNECTIONPROTOCOL_H

typedef union {
    char allBits;
    struct {
        unsigned int samplingspeed    :3; //sampling speed of oscilloscope (fixed rates)
        unsigned int oscilloscope     :1; //Oscilloscope selection 1: A, 0: B
        unsigned int gainA            :2; //gain of oscilloscope A
        unsigned int gainB            :2; //gain of oscilloscope B
        unsigned int ACDCA            :1; // AD/DC mode oscilloscope A
        unsigned int ACDCB            :1; // AC/DC mode oscilloscope B
    };
} oscilloscope_Param;

typedef union {
    char allBits;
    struct {
        unsigned int waveType   :4; //wave type select
        unsigned long int frequency  :20; // frequency
    };
} functionGenerator_Param;

typedef union {
    char allBits;
    struct {
        unsigned int oscilloscope    :1; //oscilloscope ON/OFF
        unsigned int multimeter    :1; //multimeter ON/OFF
        unsigned int functiongenerator    :1; //function generator ON/OFF
    };
} module_Select;


typedef union {
    char allBits;
    struct {
        unsigned int incomingParam  : 1; //if param incoming = 1, if module incoming = 0
        unsigned int paramForMod : 1; //0 if param are for oscilloscope, 1 if param are for functiongenerator
        unsigned int numOfParam : 2; //number of param received
    };
} param_Select;

void Parse_Data(unsigned int data);

typedef union {
    char allBits;
    struct {
        unsigned int module: 2;
        unsigned int data: 6;
    };
    struct {
        unsigned int FG_module: 2; // FG = 00
        unsigned int FG_select: 3;
        unsigned int FG_data: 3;
    };
    struct {
        unsigned int M_module: 2; //MM = 11
        unsigned int M_ON: 1;
        unsigned int M_gain: 3;
        unsigned int M_speed: 2;
    };
    struct {
        unsigned int O_module: 2; //A = 01, B = 10
        unsigned int AC_DC: 1; // AC=0, DC = 1
        unsigned int O_select: 1;
        unsigned int O_data: 3;
    };
} data_t;

#endif	/* CONNECTIONPROTOCOL_H */

