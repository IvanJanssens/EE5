#ifndef ADC_H
#define	ADC_H

#define VO_B RB5
#define ANSVO_B ANSB5
#define TRISVO_B TRISB5
#define VO_A RB14
#define ANSVO_A ANSB14
#define TRISVO_A TRISB14
#define VO_MM RD2
#define ANSVO_MM ANSD2
#define TRISVO_MM TRISD2

#define Vrefp 3.3f // +/- 5 volts
#define Vrefm 0 // 0 volts
#define resolution (4096.0f) // 12 bit

void init_ADC_A(int ADC_A);
void init_ADC_B(int ADC_B);
void init_MM(int ADC_MM);

int ADC(void ); //initialize the bits that have to do with the ADC for the pic_24

void __attribute__((__interrupt__, auto_psv )) _ADC1Interrupt(void);

#endif	/* ADC_H */

