#include <stdio.h>
#include <stdlib.h>
#include "FIFO.h"
#include "connectionprotocol.h"

data_t fifo[100];
int i;
int j;
int count;

void init_fifo(void){
    fifo = {0};
    i = 0;
    j = -1;
    count = 0;
}

void write_FIFO(unsigned char data ) {
    fifo[i] = data;
    i++;
    count++;
    if(i == 100) i = 0;
}

unsigned char read_FIFO(void) {
    j++;
    count--;
    if(j == 100) j = 0;
    return fifo[j];
}

int get_count(void) {
    return count;
}

