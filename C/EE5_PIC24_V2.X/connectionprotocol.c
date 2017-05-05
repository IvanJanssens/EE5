/*
 * File:   connectionprotocol.c
 * Author: Ivan
 *
 * Created on 4 mei 2017, 16:29
 */


#include <xc.h>

const unsigned int BitMask[] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
const unsigned int osci = 0xC0;
const unsigned int multi = 0x80;
const unsigned int funct = 0x40;
static unsigned int readBuffer[1];
static unsigned int writeBuffer[1];



void parseData(unsigned int data) {
    
                
//        if(data > 0) { //errorchecking with last 3 bits 0x40 0x20 0x10
        
//            if(data & 0x80) { //Select right state
//                if(readBuffer[0] & 0x40) {
//                    moduleSelect[7] = 1;
//                    moduleSelect[6] = 0;
//                    oscilloscopeParam[7] = readBuffer[0] & 0x20;
//                    oscilloscopeParam[6] = readBuffer[0] & 0x10;
//                    oscilloscopeParam[5] = readBuffer[0] & 0x08;
//                    ADCON0bits.GO_DONE = 1;
//                    timerLow = 0x00;
//                    timerHigh = 0xFF; //update every 0.01 sec (measure 0.005sec)
//                    TMR0H = timerHigh;
//                    TMR0L = timerLow;
//                    T0CONbits.TMR0ON = 1;
    //                putUSBUSART(readBuffer,1);
    //                updateOsci();
//                }
//                else {
//                    moduleSelect[7] = 0;
//                    moduleSelect[6] = 1;
//    //                updateMulti();
//                    ADCON0bits.GO_DONE = 1;
//                    timerLow = 0x00;
//                    timerHigh = 0x00; //update every 3.4 sec
//                    TMR0H = timerHigh;
//                    TMR0L = timerLow;
//                    T0CONbits.TMR0ON = 1;
    //                putUSBUSART(readBuffer,1);
//                }
//            }
//            else if(readBuffer[0] & multi) {
//                
//                
//            }
//            else if(readBuffer[0] & funct){
//                //get extra data for frequency
//                uint8_t databuffer [3];
//                uint8_t databytes = getsUSBUSART(databuffer, sizeof(databuffer));
//                //errorchecking on data
//                moduleSelect[7] = 0;
//                moduleSelect[6] = 0;
//                moduleSelect[5] = 1;
//                //filling in the dataparameters
////                updateFunct();
//            }
//            else if (!(readBuffer[0] & 0xff)) {
//                uint8_t databuffer [3];
////                getsUSBUSART(databuffer, sizeof(databuffer));
//                ADCON0bits.GO_DONE = 0;
//                T0CONbits.TMR0ON = 0;
//                databuffer[0] = 0x00;
//                databuffer[1] = 0xFF;
//                databuffer[2] = 0x00;
//                putUSBUSART(databuffer,3);
////                putUSBUSART(databuffer,2);
//            }
//            else {
//                ADCON0bits.GO_DONE = 0;
//                T0CONbits.TMR0ON = 0;
//                putUSBUSART(readBuffer,1);
            U2TXREG = data;
//            }
//        }
}