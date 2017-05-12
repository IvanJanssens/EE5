#include <stdio.h>
#include <stdlib.h>
#include "FIFO.h"
#include "connectionprotocol.h"

unsigned char fifo[100] = {0};
unsigned int i;
unsigned int j;
unsigned int count;

void init_FIFO(void){
    i = 0;
    j = 99;
    count = 0;
}

void write_FIFO(unsigned char data ) {
    fifo[i] = data;
    i++;
    count++;
    if(i >= 100) i = 0;
}

void read_FIFO(void) {
    while(count){
        j++;
        count--;
        if(j >= 100) j = 0;
        parse_Data(fifo[j]);
    }
}

int get_count(void) {
    return count;
}

