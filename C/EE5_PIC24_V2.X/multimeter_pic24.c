#include <xc.h>
#include "multimeter_pic24.h"
#include "connectionprotocol.h"

void MM(int value) {
    int voltage = value*LSB;
    int case_t = info.MM.gain;
    if(case_t == 0){ // /10
        if(voltage < (0.4f)){
            gain(1);
        }
        else gain(0);
    }
    else if (case_t == 1){ // 24.9k or /2 gain S2 1 S1 0 S0 1
            if( voltage > 3.0f) gain(0);
            else if ( voltage < (0.7f)) gain(2);
            else gain(1);
    }         
    else if (case_t == 2){ // 5.23k or 2x gain S2 1 S1 1 S0 1
            if( voltage > 3.0f) gain(1); 
            else if ( voltage < (0.5f)) gain(3);
            else gain(2);
    }
    else if (case_t == 3) { // 1.02k or 7x 
        if(voltage > (2.1f)) gain(2);
        else if (voltage < (0.4f)) gain(4);
        else gain(3);
    } 
    else if (case_t == 4) { // 10x
        if(voltage > (0.8f)) gain(3);
        else gain(4);
    }
}
////////////////////////////////////////////////////////////////////////////////
// cases
void gain(int cas) {
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
    info.MM.gain = cas;
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