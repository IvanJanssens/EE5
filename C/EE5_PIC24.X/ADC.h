#ifndef ADC_H
#define	ADC_H

void init_ADC(char MM, char osc_A, char osc_B);
//initialize the bits that have to do with the ADC for the pic_24

void ADC(void);

#define VO_B RB5
#define ANSVO_B ANSB5
#define TRISVO_B TRISB5
#define VO_A RB14
#define ANSVO_A ANSB14
#define TRISVO_A TRISB14
#define VO_MM RD2
#define ANSVO_MM ANSD2
#define TRISVO_MM TRISD2


#endif	/* ADC_H */

