#include <xc.h>
#include "multimeter_pic24.h"
#include "ADC.h"
////////////////////////////////////////////////////////////////////////////////
// INITCHIP
int this_case;
float LSB;

void init_MM(void)
{
    LSB = ((Vrefp - Vrefm)/(resolution));
    this_case = cases(0);
}

void MM(int value) {
    PORTB = value;  // port B are bit 2-10 from right left address
    this_case = amplify(value*LSB,this_case); // amplify will return current case
    delay(150);
}
////////////////////////////////////////////////////////////////////////////////
// amplify
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
////////////////////////////////////////////////////////////////////////////////
// cases
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
//DELAY.. 
void delay(int x ){
    int i, j, k;
    for(i = 0;i<x;i++){
            for(j = 0;j<x;j++){
                for(k = 0;k<x;k++){}}} // delay   
}
////////////////////////////////////////////////////////////////////////////////