/* 
 * File:   main.c
 * Author: Jerom Van Derbeken
 *
 * Created on March 3, 2017, 2:57 PM
 */

//#pragma config DSWDTPS = DSWDTPS1F      // Deep Sleep Watchdog Timer Postscale Select bits (1:68719476736 (25.7 Days))
//#pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select (DSWDT uses LPRC as reference clock)
//#pragma config DSBOREN = OFF            // Deep Sleep BOR Enable bit (DSBOR Disabled)
#pragma config DSWDTEN = OFF            // Deep Sleep Watchdog Timer Enable (DSWDT Disabled)
//#pragma config DSSWEN = OFF             // DSEN Bit Enable (Deep Sleep operation is always disabled)
//#pragma config RTCBAT = ON              // RTC Battery Operation Enable (RTC operation is continued through VBAT)
//#pragma config PLLDIV = DIV5            // PLL Input Prescaler Select bits (Oscillator divided by 5 (20 MHz input))
//#pragma config I2C2SEL = PRI            // Alternate I2C2 Location Select bit (I2C2 is multiplexed to SDA2/RA3 and SCL2/RA2 )
//#pragma config IOL1WAY = ON             // PPS IOLOCK Set Only Once Enable bit (Once set, the IOLOCK bit cannot be cleared)

#include <xc.h>
//#include <p24FJ128GC006.h>
//#include "config_EE5.h"



//void initChip(void);
void counter(void);
//void ADC(void);
//void initADC();

int main(void) {
    //initChip();
    //initADC();
    //ADC();
    while(1){
        TRISB = 0x00;
        counter();
        //PORTB = 0xFFFF;
        //PORTBbits.RB15 = 0;
        //PORTBbits.RB13 = 1;
    }
    
    return 0;
}

void initChip(void) {
    TRISB = 0x00;
    OSCCONbits.COSC = 2; // Primary Oscillator (XT, HS? EC)
    OSCCONbits.NOSC = 2; // HS: High Frequency
    CLKDIVbits.DOZEN = 0;  //CPU-to-peripheral clock ratio
    CLKDIVbits.PLLEN = 1;   //always active PLL
    
    RPOR5bits.RP10R = 18; // RP10 is configured as output of output compare 1
    OC1CON1 = 0;
    OC1CON1bits.OCTSEL = 7;
    
    
}

void counter(void) {
    int i = 0;
    int j = 0;
    unsigned int number = 16;
    PORTB = 1;

    while (1) {
        for (i = 0; i < 5000; i++) {
            for (j = 0; j < 6024; j++) {
            }
            number = number++;
                if (number > 256) {
                    number = 1;
                }
                PORTB = number;
        }
    }
}

//void init_ADC(void) {
//    TRISB = 0;
//    TRISBbits.TRISB0 = 1;
//    ADCON1bits.ADON = 0;
//    ADCON1 = 0x1001; // 0001 0000 0000 0001
//    ADCON2 = 0x0300; // 0000 0011 0000 0000
//    ADCON3 = 0x0001; // 0000 0000 0001 0000
//    ADCON1bits.ADON = 1;
//    while(!ADSTATHbits.ADREADY);
//}
//void ADC() {
//    
//}

