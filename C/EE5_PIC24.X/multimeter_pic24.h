#ifndef MULTIMETER_PIC24_H
#define	MULTIMETER_PIC24_H

#define COMP PORTAbits.RA1

#define Vrefp 3.3f // +/- 5 volts
#define Vrefm 0 // 0 volts
#define resolution (4096.0f) // 12 bit

#define CASE_10 1 // 0 1 1
#define CASE_2 2 // 1 0 1
#define CASE_x2 3 // 1 1 1
#define CASE_x7 4 // 1 1 0
#define CASE_x10 5 // 1 0 0

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

