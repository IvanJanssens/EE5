#include <xc.h>
#include "connectionprotocol.h"
#include "FunctionGenerator.h"
#include "ADC.h"
#include "DAC.h"
#include "FIFO.h"

info_t info;

void parse_Data(unsigned char new_data) {
    data_t data;
    data.allBits = new_data;
    switch (data.MOD.module) {
        case 0: {
            switch (data.FG.select) {
                case 0:{
                    info.FG.freq = 0;
                    info.FG.freq += data.FG.data;
                    break;
                }
                case 1:{
                    info.FG.freq |= data.FG.data<<3;
                    break;
                }
                case 2: {
                    info.FG.freq |= data.FG.data<<6;
                    break;
                }
                case 3:{
                    info.FG.freq |= data.FG.data<<9;
                    break;
                }
                case 4:{
                    info.FG.freq |= data.FG.data<<12;
                    break;
                }
                case 5:{
                    info.FG.freq += data.FG.data*32768; //2^15
                    break;
                }
                case 6:{
                    info.FG.freq += data.FG.data*262144; //2^18
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
                info.MM.ON = data.MM.ON;
                if (info.MM.ON) {
                    info.A.ON = 0;
                    info.B.ON = 0;
                    clear_tx();
                }
                write_FIFO_tx((0x07 & info.MM.gain), 0);
                OSC();
                ADC();
            }
            break;
        }
    }
    if(info.A.ON || info.B.ON || info.MM.ON) ADL0CONLbits.SLEN = 1;
}