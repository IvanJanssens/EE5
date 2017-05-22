#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <p24FJ128GC006.h>
#include "DMA.h"

void init_DMA(void) {
    DMACH0 = 0;
    DMACON = 0;
    DMACONbits.DMAEN = 1; // ENABLE DMA
    DMACONbits.PRSSEL = 1;
    DMAH = 0x850; //  (DMAINTn<7,6>) 
    DMAL = 0x1250; // CHECK THE INTERRUPT FLAGS 
    DMACH0bits.NULLW = 0; // no dumym write
    DMACH0bits.RELOAD = 1; // registers DMASRC, DMADST and DMACNT are reloaded on the start of the next operation
    DMACH0bits.SAMODE0 = 0; // DMASRC remains unchanged after a transfer completion
    DMASRC0 = (unsigned int) & source; // check header, source destination
    DMACH0bits.DAMODE0 = 1; //  DMADST is incremented based on SIZE bit after a transfer completion

    DMACH0bits.TRMODE0 = 0; // ONE SHOT
    DMACH0bits.SIZE = 0; // WORD SIZE = 16bit
    DMACH0bits.CHEN = 1; // CHANNEL ENABLE
}
// REPEATED ON SHOT MODE, RELOADS DMACNT0 AND DMADST0 AND WORKS WITH TRIGGER CHREQ. SOURCE IS SET TO ADRES0 IN DMA.h

void run_DMA(unsigned int DMADST, unsigned int DMACNT /* amount of samples / burst */) {
    DMADST0 = (unsigned short int) & DMADST;
    DMACNT0 = DMACNT;
           
    for ( dma = 0; dma < DMACNT; dma++)
    {
        DMACH0bits.CHREQ = 1; //TRIGGER
        while(DMACH0bits.CHREQ); // see info below        
    /*
    CHREQ: DMA Channel Software Request bit(3)
    1 = A DMA request is initiated by software; automatically cleared upon completion of a DMA transfer
    0 = No DMA request is pending 
    */
    }

    while (!IFS0bits.DMA0IF);
    DMAINT0bits.DONEIF = 0; //Clear DONEIF and HALIF flag
    DMAINT0bits.HALFIF = 0;
    IFS0bits.DMA0IF = 0;
}
