#include <xc.h>
#include "multimeter_pic24.h"
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
// INITCHIP
int this_case;
float LSB;

void MM(int value) {
    //PORTB = value;  // port B are bit 2-10 from right left address
    this_case = set_Case(value*LSB,this_case); // amplify will return current case
    assert(this_case != -1);
    delay(50);
}
////////////////////////////////////////////////////////////////////////////////
// amplify
int set_Case(float voltage, int this_case) {   
    if(this_case == 0){ // /10
        if(voltage < (0.4f)){
            return cases(1);
        }
        return cases(0);
    }
    else if (this_case == 1){ // 24.9k or /2 gain S2 1 S1 0 S0 1
            if( voltage > 3.0f) return cases(0);
            else if ( voltage < (0.7f)) return cases(2);
            return cases(1);
    }         
    else if (this_case == 2){ // 5.23k or 2x gain S2 1 S1 1 S0 1
            if( voltage > 3.0f) return cases(1); 
            else if ( voltage < (0.5f)) return cases(3);
            return cases(2);
    }
    else if (this_case == 3) { // 1.02k or 7x 
        if(voltage > (2.1f)) return cases(2);
        else if (voltage < (0.4f)) return cases(4);
        return cases(3);
    } 
    else if (this_case == 4) { // 10x
        if(voltage > (0.8f)) return cases(3);
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
int cases(int cas) {
    if(cas == 0) {
        LATDbits.LATD1 = 0;
        LATDbits.LATD0 = 1;
        LATDbits.LATD11 = 1;
    }
    else if(cas == 1) {
        LATDbits.LATD1 = 1;
        LATDbits.LATD0 = 0;
        LATDbits.LATD11 = 1;
    }
    else if(cas == 2) {
        LATDbits.LATD1 = 1;
        LATDbits.LATD0 = 1;
        LATDbits.LATD11 = 1;
    }
    else if (cas == 3){
        LATDbits.LATD1 = 1;
        LATDbits.LATD0 = 1;
        LATDbits.LATD11 = 0;
    }
    else if (cas == 4) {
        LATDbits.LATD1 = 1;
        LATDbits.LATD0 = 0;
        LATDbits.LATD11 = 0;
    }
    return cas;
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