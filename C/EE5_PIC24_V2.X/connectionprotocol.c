#include <xc.h>
#include "connectionprotocol.h"
#include "FunctionGenerator.h"
#include "ADC.h"

//const unsigned char ERROR = 0b11111111;

info_t info;

void parse_Data(unsigned char new_data) {

    data_t data;
    data.allBits = new_data;
    switch (data.module) {
        case 0: {
            switch (data.FG_select) {
                case 0:{
                    info.FG.bits0 = data.FG_data;
                    break;
                }
                case 1:{
                    info.FG.bits1 = data.FG_data;
                    break;
                }
                case 2: {
                    info.FG.bits2 = data.FG_data;
                    break;
                }
                case 3:{
                    info.FG.bits3 = data.FG_data;
                    break;
                }
                case 4:{
                    info.FG.bits4 = data.FG_data;
                    break;
                }
                case 5:{
                    info.FG.bits5 = data.FG_data;
                    break;
                }
                case 6:{
                    info.FG.bits6 = data.FG_data;
                    break;
                }
                default:{
                    info.FG.wave = data.FG_data;
                    break;
                }
            }
            generator();
        }
        case 1: {
            info.A.AC_DC = data.AC_DC;
            info.A.ON = data.O_ON;
            if (data.O_select) {    info.A.speed = data.O_data;    }
            else{   info.A.gain = data.O_data;   }
            init_A();
            break;
        }
        case 2: {
            info.B.AC_DC = data.AC_DC;
            info.B.ON = data.O_ON;
            if (data.O_select) {    info.B.speed = data.O_data;    }
            else{   info.B.gain = data.O_data;   }
            init_B();
            break;
        }
        default: {
            info.MM.ON = data.M_ON;
            init_MM();
            break;
        }
    }
}