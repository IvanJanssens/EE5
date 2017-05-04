/* 
 * File:   UART.h
 * Author: Ivan
 *
 * Created on 4 mei 2017, 16:30
 */

#ifndef UART_H
#define	UART_H

void uart(void);
void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void);
void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void);

#endif	/* UART_H */

