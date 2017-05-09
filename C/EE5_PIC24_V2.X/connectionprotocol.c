#include <xc.h>
#include "connectionprotocol.h"
#include "ADC.h"

const unsigned char ERROR = 0b10101010;

info_t info;

void Parse_Data(unsigned int new_data) {
    
    data_t data = new_data;
    
    if(data.module == 0){
        if(data.FG_select == 0){
            info.FG.bits0 = data.FG_data;
        }
        else if(data.FG_select == 1){
            info.FG.bits1 = data.FG_data;
        }
        else if(data.FG_select == 2){
            info.FG.bits2 = data.FG_data;
        }
        else if(data.FG_select == 3){
            info.FG.bits3 = data.FG_data;
        }
        else if(data.FG_select == 4){
            info.FG.bits4 = data.FG_data;
        }
        else if(data.FG_select == 5){
            info.FG.bits5 = data.FG_data;
        }
        else if(data.FG_select == 6){
            info.FG.bits6 = data.FG_data;
        }
        else info.FG.wave = data.FG_data;
    }
    else if (data.module == 1){
        info.A.AC_DC = data.AC_DC;
        info.A.ON = data.O_ON;
    }
//    if(param.incomingParam == 0) {
//        if(data & 0x0080) { // Module select check
//            if(data & 0x0040) { // check for 6th bit (oscilloscope on/off)
//                module.oscilloscope = 1;
//                param.incomingParam = 1;
//                param.paramForMod = 1;
//            }
//            else
//                module.oscilloscope = 0;
//            if(data & 0x0020) { // check for 5th bit (multimeter on/off)
//                module.multimeter = 1;
//            }
//            else
//                module.multimeter = log(5);
//            if(data & 0x0010) { // check for 4th bit (Functiongenerator on/off)
//                module.functiongenerator = 1;
//                param.incomingParam = 1;
//                param.paramForMod = 0;
//            }
//            else
//                module.functiongenerator = 0;
//        }
//        else if (data & 0x00AA) { //Send next oscilloscope data
//            if(module.oscilloscope == 1) {
//                
//            }
//            else {
//                U2TXREG = ERROR;
//            }
//        }
//        else {
//            U2TXREG = ERROR;
//        }
//    }
//    else { // param select
//        if( param.paramForMod == 1) {// oscilloscope param
//            if(module.oscilloscope == 1) { // check if oscilloscope is enabled
////                oscilloscopeParam.samplingspeed = data & 0x0007;
//                param.incomingParam = 0;
//            }
//            else { // else error in transmission, send error code to PC
//                U2TXREG = ERROR;
//            }
//        }
//        else { // functiongenerator param
//            if(module.functiongenerator == 1) { // check if functiongenerator is enabled
//                if(param.numOfParam == 0) {
//                    
//                }
//            }
//            else { //else error in transmission, send error code to PC
//                U2TXREG = ERROR;
//            }
//        }
//    }
//    U2TXREG = data;
    
}