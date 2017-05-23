#ifndef MULTIMETER_PIC24_H
#define	MULTIMETER_PIC24_H

#define COMP PORTAbits.RA1
#define Vrefp (3.3f) // +/- 5 volts
#define Vrefm 0 // 0 volts
#define resolution (4096.0f) // 12 bit
#define LSB ((Vrefp - Vrefm)/(resolution))

void MM(int value);
void delay(int x);
void gain(int c);

#endif	/* MULTIMETER_PIC24_H */

