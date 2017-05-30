#include <xc.h>
#include <p24FJ128GC006.h>
#include "connectionprotocol.h"
#include "FunctionGenerator.h"
#include "ADC.h"
#include "DAC.h"
#include "FIFO.h"

info_t info;
//We analyse the data and see what it means
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
            info.A.gain = data.O.gain;
            if(info.A.ON) info.MM.ON = 0;
            break;
        }
        case 2: { //10
            info.B.AC_DC = data.O.AC_DC;
            info.B.ON = data.O.ON;
            info.B.gain = data.O.gain;
            if(info.B.ON) info.MM.ON = 0;
            break;
        }
        default: { //11
            if(data.MOD.select <= 4){
                info.TRIGGER = data.TRIG.data;
            }
            else if(data.MOD.select == 4){
                info.SAMPLE = data.SAMPLE.data;
            }
            else if(data.MOD.select == 5) {
                info.CALI_ON = data.CAL.ON;
                if(info.CALI_ON){
                    info.A.ON = 1;
                    info.B.ON = 1;
                }
            }
            else if (data.MOD.select == 7) {
                info.MM.ON = data.MM.ON;
                if (info.MM.ON) {
                    info.A.ON = 0;
                    info.B.ON = 0;
                    clear_tx();
                }
            }
            break;
        }
    }
    //after we receive the data we resend the data back but only if the MM is off
    if(!info.MM.ON){
        ADL0CONHbits.SLINT = 0;
        write_FIFO_tx(255, 0);
        write_FIFO_tx(0, 0);
        write_FIFO_tx(new_data, 0);
        ADL0CONHbits.SLINT = 1;
    }
    else {
        int temp = PORTDbits.RD3;
        write_FIFO_tx(((temp*8)+(0x07 & info.MM.gain)), 0);
    }
    if(info.A.ON || info.B.ON || info.MM.ON ||info.CALI_ON ) {
        OSC();
        ADC();
        ADL0CONLbits.SLEN = (info.A.ON || info.B.ON || info.MM.ON);
    }
}