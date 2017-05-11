#ifndef FIFO_H
#define	FIFO_H

void init_fifo(void);
void write_FIFO(unsigned char data);
data_t read_FIFO(void);

int get_count(void);
#endif	/* FIFO_H */

