#ifndef UART_H
#define	UART_H

void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void);
void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void);
void InitUART2(void);
void InitPorts(void);
void uart(void);

#endif	/* UART_H */

