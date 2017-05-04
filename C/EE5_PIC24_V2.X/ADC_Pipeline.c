/********************************************************************
 FileName:      ADC_Pipeline.c
 Dependencies:  See INCLUDES section
 Processor:	    PIC24FJ128GC010 Advanced Analog Microcontroller
 Hardware:      This code is based around the analog peripherals of the
                PIC24FJ128GC010 Family of advanced analog microcontrollers.
 Complier:      Microchip C30 Compiler (of at least v3.34)
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC® Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 ********************************************************************
 File Description:

 Change History:
  Rev   Description
  ----  -----------------------------------------
  0.1   Initial version

********************************************************************/

//Includes
#include <p24Fxxxx.h>
#include "ADC_Pipeline.h"





void ADC_Pipeline_Init(unsigned int ADCReferences, unsigned int ADCFormat)
{
    unsigned int IPLSave;

    PMD1bits.ADC1MD = 0;    //Clear PMD bit to allow ADC operation
    
    //Recommended to keep ADC disabled while adjusting critical parameters 
    //like ADC clock settings
    ADCON1bits.ADON = 0;  //Disable ADC, if it was previously on
    ADCON3 = 0x0001;      //ADC clock is system clock derived
    ADCON2 = 0x0700 | ADCReferences; //Select the ADC references, buffer mode, power mode, other global settings
    ADCON1 = 0x0001 | ADCFormat; //Select ADC result formatting and other settings.
    
    //Enable the ADC module now
    ADCON1bits.ADON = 1;
    //Wait for hardware self calibration to complete
    while(ADSTATHbits.ADREADY == 0);
    
    //Apply A0 silicon workaround, to ensure self calibration gets loaded internally
    IPLSave = SR & 0x00E0;   //Save interrupt settings
    SRbits.IPL = 7; //Set CPU to max priority, so as to disable interrupts for
                    //below workaround code (which must not be interrupted to
                    //ensure it works correctly).
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;    
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;
    ADCON1bits.ADCAL = 1;  
    //Okay to restore previous interrupt settings now
    SR = (SR & 0xFF1F) | IPLSave; 
    //Wait for hardware self calibration to complete
    while(ADSTATHbits.ADREADY == 0);
    
    //The ADC is now ready to use.  Set up a sample list and generate a trigger
    //event to get result data.    
}    

void ADC_Pipeline_Disable(void)
{
    //Disable the ADC module and configure for minimum current consumption
    PMD1bits.ADC1MD = 0;    
    ADCON1bits.ADON = 0;//Disable ADC module
    ADCON2 = 0xC000;    //Reserved setting: A0 workaround to minimize IPD current.
}    



unsigned int ADC_Pipeline_GetAverageResult(unsigned int ChannelSetting, unsigned char SamplingTime, unsigned int SamplesInAverage)
{
    unsigned int SamplesRemaining = SamplesInAverage;
    unsigned char SamplesForNextAccumulation;
    unsigned int Remainder;
    unsigned int Result;
    
    //Error check input params
    if(SamplesInAverage == 0)
        return 0;
    
    //Make sure ADC sample lists and Accumulator feature are in known states
    ADCON3 &= 0xF0FF;       //Clear SLENn bits to take command of all sample lists
    ACCONH = 0x0000;        //Disable accumulation for the moment
    ACRES = 0x00000000;    //Clear previous accumulation count
    
    //Prepare Sample List 0 to include the target ChannelSetting
    ADTBL0 = ChannelSetting;
    
    //Configure Sample List 0 settings
    ADL0PTR = 0;        //Point to start of sample list 0
    ADL0CONH = 0x0000 | (SamplingTime & 0x1F);   //Set SAMC bits to the value the user passed us
    ADL0CONL = 0x0100;  //Sample list not enabled yet, SAMP = 0, Manual trigger on every Tad when ACEN is set and SAMP = 0.
    ADL0CONLbits.SLEN = 1;  //Sample list enabled!  Accumulation will commence when ACEN is also set.

    //Now begin accumulation and continue until we have the desired number of 
    //total samples
    while(SamplesRemaining != 0)
    {
        //Configure Accumulation Settings
        if(SamplesRemaining >= 256)
        {
            SamplesForNextAccumulation = 0; //0 results in 256 samples exactly
            SamplesRemaining -= 256;
        }
        else   
        {
            SamplesForNextAccumulation = SamplesRemaining;
            SamplesRemaining = 0;
        }     
        ACCONL = SamplesForNextAccumulation;    //Using sample list 0 entry 0 (ADTBL0), with SamplesForNextAccumulation number of samples
        ACCONH = 0x00C0;    //Enable accumulation feature and accumulation interrupt generation
    
        //Wait for accumulations to complete.
        while(ACCONHbits.ACEN == 1);
    }

    //We now have the results.  Divide by sample count to get the average.  We
    //use a builtin function for enhanced speed and access to remainder for rounding.
    Result = __builtin_divmodud((unsigned long int)ACRES, (unsigned int)SamplesInAverage, &Remainder);  
    //Perform rounding of result
    if(Remainder >= (SamplesInAverage >> 1))
    {
        return ++Result;    //Increment by one to round up result.
    }    
    
    return Result;
    
}    





//*******************************
//EOF ADC_Pipeline.c

