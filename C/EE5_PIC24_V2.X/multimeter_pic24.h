#ifndef MULTIMETER_PIC24_H
#define	MULTIMETER_PIC24_H

#define COMP PORTAbits.RA1

#define Vrefp 3.3f // +/- 5 volts
#define Vrefm 0 // 0 volts
#define resolution (4096.0f) // 12 bit

void init_MM(void);
int amplify( float voltage, int c);
void MM(int value);
void delay(int x);
int cases(int c);

#endif	/* MULTIMETER_PIC24_H */

