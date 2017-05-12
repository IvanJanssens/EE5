#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "FIFO.h"
#include "connectionprotocol.h"

unsigned char fifo_rx[100] = {0};
unsigned char fifo_tx[500] = {0};
unsigned int i, j, k, l;
unsigned int count_rx;
unsigned int count_tx;

void init_FIFO(void) {
    k = 0;
    l = 499;

    i = 0;
    j = 99;

    count_tx = 0;
    count_rx = 0;
}

void write_FIFO_rx(unsigned char data) {
    fifo_rx[i] = data;
    i++;
    count_rx++;
    if (i >= 100) i = 0;
}

void read_FIFO_rx(void) {
    while (count_rx) {
        j++;
        count_rx--;
        if (j >= 100) j = 0;
        parse_Data(fifo_rx[j]);
    }
}

void write_FIFO_tx(unsigned char data) {
    fifo_tx[i] = data;
    k++;
    count_tx++;
    if (k >= 500) k = 0;
}

void send_FIFO_tx(void) {
    while (count_tx) {
        l++;
        count_tx--;
        if (l >= 500) l = 0;
        U2TXREG = fifo_tx[l];
    }
}

int get_count_rx(void) {
    return count_rx;
}

int get_count_tx(void) {
    return count_tx;
}
