#ifndef MULTIMETER_PIC24_H
#define	MULTIMETER_PIC24_H

#define COMP PORTAbits.RA1

#define S2 PORTCbits.RC14
#define S1 PORTDbits.RD8
#define S0 PORTDbits.RD9

#define Vrefp 3.3f // +/- 5 volts
#define Vrefm 0 // 0 volts
#define resolution (1024.0f) // 10 bit

#define CASE_x1 1 // 0 1 1
#define CASE_x5 2 // 1 0 1
#define CASE_x20 3 // 1 1 1
#define CASE_x100 4 // 1 1 0
#define CASE_x200 5 // 1 0 0

void init_MM(void);
int amplify( float voltage, int c);
void MM(int value);
void delay(int x);
int cases(int c);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* MULTIMETER_PIC24_H */

