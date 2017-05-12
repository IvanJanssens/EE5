#ifndef FIFO_H
#define	FIFO_H

void init_FIFO(void);
void write_FIFO(unsigned char data);
void read_FIFO(void);

int get_count(void);
#endif	/* FIFO_H */

