#ifndef DMA_H
#define	DMA_H

#include <xc.h> 
#define source_A ADRES0
#define source_B ADRES1


unsigned int dma;
void init_DMA(void);
void init_DMA_A(void);
void init_DMA_B(void);
void run_DMA_A(unsigned int DMADST, unsigned int DMACNT);
void run_DMA_B(unsigned int DMADST, unsigned int DMACNT);



#endif	/* DMA_H */

