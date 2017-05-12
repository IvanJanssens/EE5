#include <xc.h>
#include "connectionprotocol.h"
#include "FIFO.h"
#include "UART.h"


void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void) {
    IFS4bits.U2ERIF = 0;
    unsigned char var = U2RXREG;
    write_FIFO_rx(var);
    U2TXREG = var;
	IFS1bits.U2RXIF = 0; // Clear the Recieve Interrupt Flag
}
void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void) {
    IFS4bits.U2ERIF = 0;
	IFS1bits.U2TXIF = 0; // Clear the Transmit Interrupt Flag
}

void init_UART2(void) {
	
    U2MODE = 0; // configure U2MODE
    U2BRG = 0x0040;           //baud rate
//	U2BRG = BAUDRATEREG2;	// baud rate
    // U2RG = Sysclk/32/Baudrate-1
    //      = 20000000/32/9600 - 1 = 64 = 0x0040
    U2STA = 0; // Load all values in for U2STA SFR
    
	IFS1bits.U2TXIF = 0;	// Clear the Transmit Interrupt Flag
	IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts
	IFS1bits.U2RXIF = 0;	// Clear the Recieve Interrupt Flag
	IEC1bits.U2RXIE = 1;	// Enable Recieve Interrupts

	U2MODEbits.UARTEN = 1;	// And turn the peripheral on

	U2STAbits.UTXEN = 1;
}

void init_Ports(void) {
    TRISF = 0x0008; // pin RF3 is input
    ANSFbits.ANSF3 = 0; // pin RF3 is digital (not analog)
}


void set_UART(void) {
    init_Ports();
	// I/O remap, PPS
	// Unlock Registers
	__builtin_write_OSCCONL(OSCCON & 0xbf);
	// Configure Input Functions **********************
	// Assign UART2RX To Pin RP16 (RF3)
	RPINR19bits.U2RXR = 16;
	// Configure Output Functions *********************
	// Assign UART2TX To Pin RP17 (RF5)
	RPOR8bits.RP17R = 5;
	// Lock Registers
	__builtin_write_OSCCONL(OSCCON | 0x40);

	init_UART2();	// Initialize UART2 for 9600,8,N,1 TX/RX

}
