#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "FIFO.h"
#include <assert.h>
#include "connectionprotocol.h"

unsigned char fifo_rx[50] = {0};
unsigned int fifo_tx[max_fifo] = {0};
unsigned int i, j, k, l;
unsigned int count_rx;
unsigned int count_tx;

void init_FIFO(void) {
    k = 0;
    l = max_fifo;
    i = 0;
    j = 100;

    count_tx = 0;
    count_rx = 0;
}

void write_FIFO_rx(unsigned char data) {
    if(count_rx < 100){
        fifo_rx[i] = data;
        i++;
        count_rx++;
        if (i >= 100) i = 0;
    }
}

void read_FIFO_rx(void) {
    while (count_rx) {
        j++;
        count_rx--;
        if (j >= 100) j = 0;
        parse_Data(fifo_rx[j]);
    }
}

void write_FIFO_tx(unsigned int data, char input){
    if(count_tx < max_fifo){
        fifo_tx[k] = (data & 0xFFF) + input*4096; // 2^12 = 4096
        k++;
        count_tx++;
        if (k >= max_fifo) k = 0;
    }
}

void send_FIFO_tx(void) {
    while (count_tx) {
        l++;
        count_tx--;
        if (l >= max_fifo) l = 0;
        if((fifo_tx[l] & 0xF000) == 0x0) {
            while(U2STAbits.UTXBF);
            U2TXREG = fifo_tx[l];
        }
        else if((fifo_tx[l] & 0xF000) == 0x1000) {
            int var = (fifo_tx[l] & 0xFFF);
            int var1 = (0xF80 & var)/128;
            int var2 = (0x7C & var)/4;
            while(U2STAbits.UTXBF);
            U2TXREG = 64 | (var1 & 0x1F); //0101 1111
            while(U2STAbits.UTXBF);
            U2TXREG = 96 | (var2 & 0x1F); //0111 1111
        }
        else if((fifo_tx[l] & 0xF000) == 0x2000){
            int var = (fifo_tx[l] & 0xFFF);
            int var1 = (0x0F80 & var)/128;
            int var2 = (0x007C & var)/4;
            while(U2STAbits.UTXBF);
            U2TXREG = 128 | (var1 & 0x1F); //1001 1111
            while(U2STAbits.UTXBF);
            U2TXREG = 160 | (var2 & 0x1F); //1011 1111
        }
        else if((fifo_tx[l] & 0xF000) == 0x3000){
            int var = (fifo_tx[l] & 0xFFF);
            int var1 = (0x0F80 & var)/128;
            int var2 = (0x7C & var);
            while(U2STAbits.UTXBF);
            U2TXREG = (0xC0 | (var1 & 0x1F));
            while(U2STAbits.UTXBF);
            U2TXREG = (0xE0 | (var2 & 0x1F));
        }
    }
}

void clear_tx(void){
    count_tx = 0;
}

int get_count_rx(void) {
    return count_rx;
}

int get_count_tx(void) {
    return count_tx;
}
