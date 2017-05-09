#ifndef MULTIMETER_PIC24_H
#define	MULTIMETER_PIC24_H

#define COMP PORTAbits.RA1

void init_MM(void);
int set_Case( float voltage, int c);
void MM(int value);
void delay(int x);
int cases(int c);

#endif	/* MULTIMETER_PIC24_H */

