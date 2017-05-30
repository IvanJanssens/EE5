#include <xc.h>
#include "DMA.h"
#include "connectionprotocol.h"
#include "fifo.h"

void init_DMA(void) {
      
    
    DMACH0bits.SAMODE0 = 0; // DMASRC remains unchanged after a transfer completion
    DMACH0bits.DAMODE0 = 1; //  DMADST is incremented based on SIZE bit after a transfer completion
    DMACH0bits.NULLW = 0; // no dumym write
    DMACH0bits.RELOAD = 1; // registers DMASRC, DMADST and DMACNT are reloaded on the start of the next operation
    DMAINT0bits.CHSEL = 23; //10111
    DMACH0bits.TRMODE0 = 1; // REPEATED ONE SHOT
    DMACH0bits.SIZE = 0; // WORD SIZE = 16bit
    
    DMACH1bits.SAMODE0 = 0; // DMASRC remains unchanged after a transfer completion
    DMACH1bits.DAMODE0 = 1; //  DMADST is incremented based on SIZE bit after a transfer completion
    DMAINT1bits.CHSEL = 23; //10111
    DMACH1bits.NULLW = 0; // no dumym write
    DMACH1bits.RELOAD = 1; // registers DMASRC, DMADST and DMACNT are reloaded on the start of the next operation
    DMACH1bits.TRMODE0 = 1; // REPEATED ONE SHOT
    DMACH1bits.SIZE = 0; // WORD SIZE = 16bit
    
    DMAH = 0x850; //  (DMAINTn<7,6>) 
    DMAL = 0x1250; // CHECK THE INTERRUPT FLAGS 
    
    if (info.A.ON && info.B.ON) {
        //A, CH0
        DMASRC0 = (unsigned int)& ADRES0;
        DMADST0 = (unsigned int)& 0; // fifo A ;
        DMACNT0 = max_fifo;
        ///B, CH1
        DMASRC1 = (unsigned int) & ADRES1;
        DMADST1 = 0; // fifo B;
        DMACNT1 = max_fifo;
    } else if (info.A.ON) {
        // CH A
        DMASRC0 = (unsigned int) & ADRES0;
        DMADST0 = 0; // fifo A ;
        DMACNT0 = max_fifo;
    } else {
        // CH B
        DMADST0 = 0; // fifo B ;
        DMASRC0 = (unsigned int) & ADRES0;
        DMACNT0 = max_fifo;
    }
    
    DMACH0bits.CHEN = 1; // CHANNEL ENABLE
    DMACH1bits.CHEN = 1; // CHANNEL ENABLE
}

void clear_DMA(void) {
    DMACH0bits.CHEN = 0; // CHANNEL ENABLE
    DMACH1bits.CHEN = 0; // CHANNEL ENABLE
}