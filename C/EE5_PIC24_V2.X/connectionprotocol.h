#ifndef CONNECTIONPROTOCOL_H
#define	CONNECTIONPROTOCOL_H

typedef union {
    unsigned int allBits;
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
        unsigned int O_module: 2; //A = 01, B = 10
        unsigned int O_ON: 1;
        unsigned int AC_DC: 1; // AC=0, DC = 1
        unsigned int O_select: 1;
        unsigned int O_data: 3;
    };
    struct {
        unsigned int M_module: 2; //MM = 11
        unsigned int M_ON: 1;
        unsigned int M_gain: 3;
    };
} data_t;

typedef struct {
    struct {
        unsigned int ON: 1;
        unsigned int AC_DC: 1;
        unsigned int gain: 3;
        unsigned int speed: 3;
    } A;
    struct {
        unsigned int ON: 1;
        unsigned int AC_DC: 1;
        unsigned int gain: 3;
        unsigned int speed: 3;
    } B;
    struct {
        unsigned int ON: 1;
        unsigned int gain: 3;
    } MM;
    struct {
        unsigned int wave: 3;
        union {
            long int allBits;
            unsigned int bits0: 3;
            unsigned int bits1: 3;
            unsigned int bits2: 3;
            unsigned int bits3: 3;
            unsigned int bits4: 3;
            unsigned int bits5: 3;
            unsigned int bits6: 3;
        };
    } FG;
} info_t;

void Parse_Data(unsigned int data);

extern info_t info;

#endif	/* CONNECTIONPROTOCOL_H */

