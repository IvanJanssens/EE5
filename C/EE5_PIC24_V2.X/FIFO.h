#ifndef FIFO_H
#define	FIFO_H

#define max_fifo 3000 //can be rised to 8000

void init_FIFO(void);
void write_FIFO_rx(unsigned char data);
void read_FIFO_rx(void);

void write_FIFO_tx(unsigned int data, char input);
void send_FIFO_tx(void);

void clear_tx(void);
int get_count_rx(void);
int get_count_tx(void);
#endif	/* FIFO_H */

