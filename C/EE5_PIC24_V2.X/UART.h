#ifndef UART_H
#define	UART_H

void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void);
void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void);
void init_UART2(void);
void init_Ports(void);
void set_UART(void);

#endif	/* UART_H */

