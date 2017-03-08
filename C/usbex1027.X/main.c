/** INCLUDES *******************************************************/
#include "system.h"
#include "system_config.h"
#include "app_led_usb_status.h"
#include "usb_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_cdc.h"


static uint8_t readBuffer[64];
static uint8_t writeBuffer[64];
void APP_DeviceCDCBasicDemoTasks();
void APP_DeviceCDCBasicDemoInitialize();
void UserInit();
void interrupt low_priority low_isr();   //low priority interrupt routine
void interrupt SYS_InterruptHigh();

/*
 * Global Variable
 */
uint8_t ad[3];
uint8_t timerLow;
uint8_t timerHigh;

MAIN_RETURN main(void)
{
    timerLow = 0x00;
    timerHigh = 0x00;
    ad[0] = 1;
    ad[1] = 0;
    ad[2] = 0;
    SYSTEM_Initialize(SYSTEM_STATE_USB_START);

    USBDeviceInit();
    USBDeviceAttach();
    UserInit();
    while(1)
    {
        SYSTEM_Tasks();
        //Application specific tasks
        APP_DeviceCDCBasicDemoTasks();

    }//end while
}//end main

/*User defined Initialiaze Function*/
void UserInit()
{
    PORTA = 0x00;
    TRISA = 0x01;
    PORTB = 0x00;
    TRISB = 0x02;
    CMCON = 0x07;	
//    ADCON1 = 0x0f;
    
    
    T0CON = 0x07;
    TMR0H = 0x00;
    TMR0L = 0x00;
    INTCON= 0x20;
    
    ADCON0 = 0x01;
    ADCON1 = 0x00;
    ADCON2 = 0x3E;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    INTCONbits.GIE = 1;
}

void interrupt low_priority low_isr()
{ 
    
}

#if defined(__XC8)
void interrupt SYS_InterruptHigh(void)
{
    if(INTCONbits.TMR0IF == 1)
     {
        ADCON0bits.GO_DONE = 1;
         
        TMR0H = timerHigh;
        TMR0L = timerLow;    //reload the value to the Timer0
        INTCONbits.TMR0IF=0;     //CLEAR interrupt flag when you are done!!!
     } 
    else if(PIR1bits.ADIF == 1)
    {
        ad[1] = ADRESH;
        ad[2] = ADRESL;
        putUSBUSART(ad,3);
        PIR1bits.ADIF = 0;
    }
    else {
        #if defined(USB_INTERRUPT)
            USBDeviceTasks();
        #endif
    }
    
}
#else
   
   
#endif



void APP_DeviceCDCBasicDemoTasks()
{
    /* Check to see if there is a transmission in progress, if there isn't, then
     * we can see about performing an echo response to data received.
     */
    if( USBUSARTIsTxTrfReady() == true)
    {
        uint8_t i;
        uint8_t numBytesRead;
        /*get the data from the computer,and put in an array*/
        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));

        
        /* For every byte that was read... */
        for(i=0; i<numBytesRead; i++)
        {
            switch(readBuffer[i])
            {
                /* If we receive new line or line feed commands, just echo
                 * them direct.
                 */
                case 0x30:
                    T0CONbits.TMR0ON = 0;
                    break;
                case 0x39:
                    ADCON0bits.GO_DONE = 1;
                    timerLow = 0x00;
                    timerHigh = 0x00; //update every 3.4 sec
                    TMR0H = timerHigh;
                    TMR0L = timerLow;
                    T0CONbits.TMR0ON = 1;
                    break;
                case 0x38:
                    ADCON0bits.GO_DONE = 1;
                    timerLow = 0x00;
                    timerHigh = 0xFF; //update every 0.01 sec
                    TMR0H = timerHigh;
                    TMR0L = timerLow;
                    T0CONbits.TMR0ON = 1;
                    break;
                case 0x0A:
                case 0x31:
                    PORTBbits.RB7 = 1;
                    PORTBbits.RB6 = 0;
                    PORTBbits.RB5 = 0;
                    writeBuffer[i] = readBuffer[i];
                    break;
                case 0x32:
                    PORTBbits.RB7 = 1;
                    PORTBbits.RB6 = 1;
                    PORTBbits.RB5 = 0;
                    writeBuffer[i] = readBuffer[i];
                    break;
                case 0x33:
                    PORTBbits.RB7 = 1;
                    PORTBbits.RB6 = 1;
                    PORTBbits.RB5 = 1;
                    writeBuffer[i] = readBuffer[i];
                    break;
                case 0x0D:
                    writeBuffer[i] = readBuffer[i];
                    break;
                /* If we receive something else, then echo it plus one
                 * so that if we receive 'a', we echo 'b' so that the
                 * user knows that it isn't the echo enabled on their
                 * terminal program.
                 */
                default:
                    PORTBbits.RB7 = 0;
                    PORTBbits.RB6 = 0;
                    PORTBbits.RB5 = 0;
                    writeBuffer[i] = readBuffer[i] + 1;
                    break;
            }
        }

        if(numBytesRead > 0)
        {
            /* After processing all of the received data, we need to send out
             * the "echo" data now.
             */
            putUSBUSART(writeBuffer,numBytesRead);
//            putUSBUSART(ad,3);
//            if(ADCON0bits.GO_DONE == 0)
//                ADCON0bits.GO_DONE = 1;
        }
    }

    CDCTxService();
}

/*You don't need to care the code below*/
void APP_DeviceCDCBasicDemoInitialize()
{
    CDCInitEP();


    line_coding.bCharFormat = 0;
    line_coding.bDataBits = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate = 9600;

}

bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    switch( (int) event )
    {
        case EVENT_TRANSFER:
            break;

        case EVENT_SOF:
            /* We are using the SOF as a timer to time the LED indicator.  Call
             * the LED update function here. */
            APP_LEDUpdateUSBStatus();
            break;

        case EVENT_SUSPEND:
            /* Update the LED status for the suspend event. */
            APP_LEDUpdateUSBStatus();
            break;

        case EVENT_RESUME:
            /* Update the LED status for the resume event. */
            APP_LEDUpdateUSBStatus();
            break;

        case EVENT_CONFIGURED:
            /* When the device is configured, we can (re)initialize the
             * demo code. */
           APP_DeviceCDCBasicDemoInitialize();
            break;

        case EVENT_SET_DESCRIPTOR:
            break;

        case EVENT_EP0_REQUEST:
            /* We have received a non-standard USB request.  The HID driver
             * needs to check to see if the request was for it. */
            USBCheckCDCRequest();
            break;

        case EVENT_BUS_ERROR:
            break;

        case EVENT_TRANSFER_TERMINATED:
            break;

        default:
            break;
    }
    return true;
}
//        /* If the USB device isn't configured yet, we can't really do anything
//         * else since we don't have a host to talk to.  So jump back to the
//         * top of the while loop. */
//        if( USBGetDeviceState() < CONFIGURED_STATE )
//        {
//            /* Jump back to the top of the while loop. */
//            continue;
//        }

//        /* If we are currently suspended, then we need to see if we need to
//         * issue a remote wakeup.  In either case, we shouldn't process any
//         * keyboard commands since we aren't currently communicating to the host
//         * thus just continue back to the start of the while loop. */
//        if( USBIsDeviceSuspended()== true )
//        {
//            /* Jump back to the top of the while loop. */
//            continue;
//        }

/*******************************************************************************
 End of File
*/
