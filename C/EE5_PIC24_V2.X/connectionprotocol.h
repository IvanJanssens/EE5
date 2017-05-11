#ifndef CONNECTIONPROTOCOL_H
#define	CONNECTIONPROTOCOL_H

typedef union {
    unsigned char allBits;

    struct {
        unsigned char module : 2;
        unsigned char data : 6;
    };

    struct {
        unsigned char FG_module : 2; // FG = 00
        unsigned char FG_select : 3;
        unsigned char FG_data : 3;
    };

    struct {
        unsigned char O_module : 2; //A = 01, B = 10
        unsigned char O_ON : 1;
        unsigned char AC_DC : 1; // AC=0, DC = 1
        unsigned char O_select : 1;
        unsigned char O_data : 3;
    };

    struct {
        unsigned char M_module : 2; //MM = 11
        unsigned char M_ON : 1;
        unsigned char M_nothing : 5;
    };
} data_t;

typedef union {
    long long int allbits; //64bits
    struct { // 3*8 + 35 = 57 bit
        struct { // 8bit
            unsigned char ON : 1;
            unsigned char AC_DC : 1;
            unsigned char gain : 3;
            unsigned char speed : 3;
        } A;

        struct { //8bit
            unsigned char ON : 1;
            unsigned char AC_DC : 1;
            unsigned char gain : 3;
            unsigned char speed : 3;
        } B;

        struct { //8bit
            unsigned char ON : 1;
            unsigned char nothing : 7;
        } MM;

        struct { // 35bit
            unsigned char wave : 3;
            union {
                long int allBits; //32bit
                struct { //32 bit
                    unsigned int nothing : 12;
                    unsigned char bits6 : 2;
                    unsigned char bits5 : 3;
                    unsigned char bits4 : 3;
                    unsigned char bits3 : 3;
                    unsigned char bits2 : 3;
                    unsigned char bits1 : 3;
                    unsigned char bits0 : 3;
                };
            };
        } FG;
    };
} info_t;

void Parse_Data(unsigned int data);

extern info_t info;

#endif	/* CONNECTIONPROTOCOL_H */

