#ifndef UART_H
#define	UART_H

void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void);
void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void);
void Init_UART2(void);
void Init_Ports(void);
void SET_UART(void);

#endif	/* UART_H */

