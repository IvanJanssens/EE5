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

extern char A, B, M;

void init_ADC();
//initialize the bits that have to do with the ADC for the pic_24

char ADC(void );

#endif	/* ADC_H */

