#include <xc.h>
#include "connectionprotocol.h"
#include "UART.h"

void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void) {
    IFS4bits.U2ERIF = 0;
    unsigned int uart_RX = U2RXREG;
    parseData(uart_RX);
//	U2TXREG = uart_RX;
	IFS1bits.U2RXIF = 0;
}
void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void) {
    IFS4bits.U2ERIF = 0;
	IFS1bits.U2TXIF = 0;
}

void InitUART2(void) 
{

	// configure U2MODE
	U2MODEbits.UARTEN = 0;	// Bit15 TX, RX DISABLED, ENABLE at end of func
	U2MODEbits.USIDL = 0;	// Bit13 Continue in Idle
	U2MODEbits.IREN = 0;	// Bit12 No IR translation
	U2MODEbits.RTSMD = 0;	// Bit11 Simplex Mode
	U2MODEbits.UEN = 0;		// Bits8,9 TX,RX enabled, CTS,RTS not
	U2MODEbits.WAKE = 0;	// Bit7 No Wake up (since we don't sleep here)
	U2MODEbits.LPBACK = 0;	// Bit6 No Loop Back
	U2MODEbits.ABAUD = 0;	// Bit5 No Autobaud (would require sending '55')
	U2MODEbits.RXINV = 0;	// Bit4 IdleState = 1
	U2MODEbits.BRGH = 0;	// Bit3 16 clocks per bit period
	U2MODEbits.PDSEL = 0;	// Bits1,2 8bit, No Parity
	U2MODEbits.STSEL = 0;	// Bit0 One Stop Bit
	
//	U2BRG = BAUDRATEREG2;	// baud rate
    U2BRG = 0x0040;           //baud rate
    // U2RG = Sysclk/32/Baudrate-1
    //      = 20000000/32/9600 - 1 = 64 = 0x0040

	// Load all values in for U2STA SFR
	U2STAbits.UTXISEL1 = 0;	//Bit15 Int when Char is transferred (1/2 config!)
	U2STAbits.UTXINV = 0;	//Bit14 N/A, IRDA config
	U2STAbits.UTXISEL0 = 0;	//Bit13 Other half of Bit15
	U2STAbits.UTXBRK = 0;	//Bit11 Disabled
	U2STAbits.UTXEN = 0;	//Bit10 TX pins controlled by periph
	U2STAbits.UTXBF = 0;	//Bit9 *Read Only Bit*
	U2STAbits.TRMT = 0;		//Bit8 *Read Only bit*
	U2STAbits.URXISEL = 0;	//Bits6,7 Int. on character recieved
	U2STAbits.ADDEN = 0;	//Bit5 Address Detect Disabled
	U2STAbits.RIDLE = 0;	//Bit4 *Read Only Bit*
	U2STAbits.PERR = 0;		//Bit3 *Read Only Bit*
	U2STAbits.FERR = 0;		//Bit2 *Read Only Bit*
	U2STAbits.OERR = 0;		//Bit1 *Read Only Bit*
	U2STAbits.URXDA = 0;	//Bit0 *Read Only Bit*

	IFS1bits.U2TXIF = 0;	// Clear the Transmit Interrupt Flag
	IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts
	IFS1bits.U2RXIF = 0;	// Clear the Recieve Interrupt Flag
	IEC1bits.U2RXIE = 1;	// Enable Recieve Interrupts

	U2MODEbits.UARTEN = 1;	// And turn the peripheral on

	U2STAbits.UTXEN = 1;
}

void InitPorts(void) 
{
    TRISF = 0x0008; // pin RF3 is input
    ANSFbits.ANSF3 = 0; // pin RF3 is digital (not analog)
}


void uart(void) {
    InitPorts();
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

	InitUART2();	// Initialize UART2 for 9600,8,N,1 TX/RX


}
