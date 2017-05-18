#include <xc.h>
#include "connectionprotocol.h"
#include "FunctionGenerator.h"
#include "ADC.h"
#include "DAC.h"

info_t info;

void parse_Data(unsigned char new_data) {
    data_t data;
    data.allBits = new_data;
    switch (data.MOD.module) {
        case 0: {
            switch (data.FG.select) {
                case 0:{
                    info.FG.bits0 = data.FG.data;
                    break;
                }
                case 1:{
                    info.FG.bits1 = data.FG.data;
                    break;
                }
                case 2: {
                    info.FG.bits2 = data.FG.data;
                    break;
                }
                case 3:{
                    info.FG.bits3 = data.FG.data;
                    break;
                }
                case 4:{
                    info.FG.bits4 = data.FG.data;
                    break;
                }
                case 5:{
                    info.FG.bits5 = data.FG.data;
                    break;
                }
                case 6:{
                    info.FG.bits6 = data.FG.data;
                    break;
                }
                default:{
                    info.FG.wave = data.FG.data;
                    generator();
                    break;
                }
            }
            break;
        }
        case 1: { //01
            info.A.AC_DC = data.O.AC_DC;
            info.A.ON = data.O.ON;
            if(info.A.ON) info.MM.ON = 0;
            info.A.gain = data.O.gain;
            OSC();
            ADC();
            break;
        }
        case 2: { //10
            info.B.AC_DC = data.O.AC_DC;
            info.B.ON = data.O.ON;
            if(info.B.ON) info.MM.ON = 0;
            info.B.gain = data.O.gain;
            OSC();
            ADC();
            break;
        }
        default: { //11
            if(data.MOD.select == 1)info.A.offset = data.DAC.data;
            else if(data.MOD.select == 2) info.B.offset = data.DAC.data;
            else if (data.MOD.select == 3) {
                info.B.ON = data.MM.ON;
                if (info.MM.ON) {
                info.A.ON = 0;
                info.B.ON = 0;
                }
                OSC();
                ADC();
            }
            break;
        }
    }
}