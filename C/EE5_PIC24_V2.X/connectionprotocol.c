#include <xc.h>
#include "connectionprotocol.h"

const unsigned char ERROR = 0b10101010;


union _moduleSelect {
    char allBits;
    struct {
        unsigned int oscilloscope    :1; //oscilloscope ON/OFF
        unsigned int multimeter    :1; //multimeter ON/OFF
        unsigned int functiongenerator    :1; //function generator ON/OFF
    };
} moduleSelect;


union _paramSelect {
    char allBits;
    struct {
        unsigned int incomingParam  : 1; //if param incoming = 1, if module incoming = 0
        unsigned int paramForMod : 1; //0 if param are for oscilloscope, 1 if param are for functiongenerator
        unsigned int numOfParam : 2; //number of param received
    };
} paramSelect;


void parseData(unsigned int data) {
    
    if(paramSelect.incomingParam == 0) {
        if(data & 0x0080) { // Module select check
            if(data & 0x0040) { // check for 6th bit (oscilloscope on/off)
                moduleSelect.oscilloscope = 1;
                paramSelect.incomingParam = 1;
                paramSelect.paramForMod = 1;
            }
            else
                moduleSelect.oscilloscope = 0;
            if(data & 0x0020) { // check for 5th bit (multimeter on/off)
                moduleSelect.multimeter = 1;
            }
            else
                moduleSelect.multimeter = 0;
            if(data & 0x0010) { // check for 4th bit (Functiongenerator on/off)
                moduleSelect.functiongenerator = 1;
                paramSelect.incomingParam = 1;
                paramSelect.paramForMod = 0;
            }
            else
                moduleSelect.functiongenerator = 0;
        }
        else if (data & 0x00AA) { //Send next oscilloscope data
            if(moduleSelect.oscilloscope == 1) {
                
            }
            else {
                U2TXREG = ERROR;
            }
        }
        else {
            U2TXREG = ERROR;
        }
    }
    else { // param select
        if( paramSelect.paramForMod == 1) {// oscilloscope param
            if(moduleSelect.oscilloscope == 1) { // check if oscilloscope is enabled
//                oscilloscopeParam.samplingspeed = data & 0x0007;
                paramSelect.incomingParam = 0;
            }
            else { // else error in transmission, send error code to PC
                U2TXREG = ERROR;
            }
        }
        else { // functiongenerator param
            if(moduleSelect.functiongenerator == 1) { // check if functiongenerator is enabled
                if(paramSelect.numOfParam == 0) {
                    
                }
            }
            else { //else error in transmission, send error code to PC
                U2TXREG = ERROR;
            }
        }
    }
    U2TXREG = data;
}