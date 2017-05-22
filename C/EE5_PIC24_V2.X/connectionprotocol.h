#ifndef CONNECTIONPROTOCOL_H
#define	CONNECTIONPROTOCOL_H

typedef union {
    unsigned char allBits;
    struct {
        unsigned char data : 4;
        unsigned char select: 2;
        unsigned char module : 2;
    }MOD;
    
    struct {
        unsigned char data : 3;
        unsigned char select : 3;
        unsigned char module : 2; // FG = 00
    } FG ;

    struct {
        unsigned char gain : 4;
        unsigned char AC_DC : 1; // AC=0, DC = 1
        unsigned char ON : 1;
        unsigned char module : 2; //A = 01, B = 10
    } O ;
    struct {
        unsigned char nothing: 3;
        unsigned char ON : 1;
        unsigned char select : 2; //A => 01, B = 10, MM = 11
        unsigned char module : 2; //11
    } MM;
    struct {
        unsigned char data : 4; 
        unsigned char select : 2; //A => 01, B = 10, MM = 11
        unsigned char module : 2; //11
    } DAC;
} data_t;

typedef union {
    long long int allbits; //64bits
    struct { 
        struct { //9bit
            unsigned char offset: 4;
            unsigned char gain : 3;
            unsigned char AC_DC : 1;
            unsigned char ON : 1;
        } A;

        struct { //9bit
            unsigned char offset: 4;
            unsigned char gain : 3;
            unsigned char AC_DC : 1;
            unsigned char ON : 1;
        } B;
        
        struct { //4bit
            unsigned char gain : 3;
            unsigned char ON : 1;
        } MM;

        struct { // 23bit
            unsigned long int freq:21;
            unsigned char wave : 3;
        } FG;
    };
} info_t;

void parse_Data(unsigned char data);

extern info_t info;

#endif	/* CONNECTIONPROTOCOL_H */

