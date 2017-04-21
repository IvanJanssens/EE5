/*
 * File:   mm_main.c
 * Author: Kris
 *
 * Created on 25 februari 2017, 12:42
 */


#include <xc.h>

#define S2 PORTAbits.RA5
#define S1 PORTAbits.RA4
#define S0 PORTAbits.RA3
#define COMP PORTAbits.RA1


#define Vrefp 5 // +/- 5 volts
#define Vrefm 0 // 0 volts
#define resolution (1024.0f) // 10 bit


#define CASE_x1 1 // 0 1 1
#define CASE_x5 2 // 1 0 1
#define CASE_x20 3 // 1 1 1
#define CASE_x100 4 // 1 1 0
#define CASE_x200 5 // 1 0 0


////////////////////////////////////////////////////////////////////////////////
// FUNCTION DECLARATIONS
void interrupt ISR (void); void initChip(void);
int amplify( float voltage, int c);
float calc(int h, int l);
void delay(int x);
int cases(int c);
////////////////////////////////////////////////////////////////////////////////
// VARIABLE DECLARATIONS
int ADF = 0;
char H_C, L_C;
float LSB;
////////////////////////////////////////////////////////////////////////////////
// MAIN
void main(void) {
    initChip();
    int this_case = cases(0);
    
    LSB = ((Vrefp - Vrefm)/(resolution)); 
    
    while(1)    //Endless loop
    {       
        delay(1);
        if (ADF  == 1){
            PORTB = H_C;  // port B are bit 2-10 from right left adress
            this_case = amplify(calc(H_C,L_C),this_case); // amplify will return current case
            delay(150);
            ADF = 0;
                       
            ADCON0bits.GO_DONE = 1;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
//DELAY.. 
void delay(int x ){
   for(int i = 0;i<x;i++){
            for(int j = 0;j<x;j++){
                for(int k = 0;k<x;k++){}}} // delay   
}

////////////////////////////////////////////////////////////////////////////////
// INITCHIP
void initChip(void)
{
    INTCON = 0xC0;
    PIE1bits.ADIE = 1;  // enable AD ir 
    PORTA = 0x00; //Initial PORTA
    TRISA = 0x03; //Define PORTA as 0000 0011; A0 is mm uitgang
    
    PORTB = 0x00; //Initial PORTB
    TRISB = 0x00; //Define PORTB as output
    TRISB = 0x00; //Define PORTB as output
    
    ADCON0 = 0x00;
    ADCON0 = 0x01; //Initial ADCON0, channel AN0/RA0 0000 0011
    ADCON1 = 0x0F; //Turn off ADcon
    ADCON1 = 0x0E; //configure A0 as analog 0000 1110
    ADCON2 = 0x3E; // 0 0 1 1 1 1 1 0
    ADCON0bits.GO_DONE = 1;
}
////////////////////////////////////////////////////////////////////////////////
// INTERRUPT
void interrupt ISR(void) {       
    if (PIR1bits.ADIF == 1 ){
        H_C = ADRESH;
        L_C = ADRESL;
        PIR1bits.ADIF = 0;
        ADF = 1;
    } 
}
////////////////////////////////////////////////////////////////////////////////
// THIS FUNCTION CALCULATES THE VOLTAGE AT THE ADC IN FLOAT VALUE   
float calc(  int h,  int l ){ // summing of ADRESH and ADRESL   
    h = h*2*2; // shift left twice
    l = l/(2*2*2*2*2*2); // shift right six 
    return (h+l)*LSB;
 }

int amplify(float voltage, int this_case) {   
    if(this_case == 0){ // unity gain S2 0 S1 1 S0 1
        if(voltage < (0.3f)){
            cases(1);
            return 1;
        }
        return cases(0);
    }
    else if (this_case == 1){ // 24.9k or /2 gain S2 1 S1 0 S0 1
            if( voltage > 2) return cases(0);
            else if ( voltage < (0.8f)) return cases(2);
            return cases(1);
    }         
    else if (this_case == 2){ // 5.23k or 2x gain S2 1 S1 1 S0 1
            if( voltage > 4) return cases(1); 
            else if ( voltage < (0.7f)) return cases(3);
            return cases(2);
    }
    else if (this_case == 3) { // 1.02k or 7x 
        if(voltage > (3.5f)) return cases(2);
        else if (voltage < (0.9f)) return cases(4);
        return cases(3);
    } 
    else if (this_case == 4) {
        if(voltage > (2)) return cases(3);
        return cases(4);
    }else {
        while(1){
            PORTB = 255;
        }   
        return -1;
    }
}
            /*
        case CASE_x100:  // 1.02k ohm or 10x gain S2 1 S1 1 S0 0
            if( voltage > (3.15f)){
                
                S2 = 1; 
                S1 = 1; 
                S0 = 1; 
                current_case = CASE_x20;}
            else if ( voltage < (0.8f))
                {   
                   
                S2 = 1; 
                S1 = 0; 
                S0 = 0; 
                current_case = CASE_x200;} 
            
            break;
            
        case CASE_x200: // 500 ohm or 20x gain S2 1 S1 0 S0 0
            if ( voltage > (0.9f)){
                
                S2 = 1; 
                S1 = 1; 
                S0 = 0; 
                current_case = CASE_x100;}
            
            break; */

int cases(int c) {
    if(c == 0) { // /10
        S0 = 1;
        S1 = 1;
        S2 = 0;
    }
    else if(c == 1) { // /2
        S0 = 1;
        S1 = 0;
        S2 = 1;
    }
    else if(c == 2) { //X2
        S0 = 1;
        S1 = 1;
        S2 = 1;
    }
    else if (c == 3){ // X7
        S0 = 0;
        S1 = 1;
        S2 = 1;
    }
    else if (c == 4) { // X10
        S0 = 0;
        S1 = 0;
        S2 = 1;
    }
    else {
        S0 = 0;
        S1 = 0;
        S2 = 0;
        c = -1;
    }
    return c;
}
////////////////////////////////////////////////////////////////////////////////