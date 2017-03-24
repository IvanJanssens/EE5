/********************************************************************
 FileName:      ADC_SigmaDelta.h
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
 ********************************************************************/

#ifndef __ADC_PIPELINE__H
#define __ADC_PIPELINE__H



//------------------------------------------------------------------------------
//Public prototypes
//------------------------------------------------------------------------------
void ADC_Pipeline_Init(unsigned int ADCReferences, unsigned int ADCFormat);
void ADC_Pipeline_Disable(void);
unsigned int ADC_Pipeline_GetAverageResult(unsigned int ChannelSetting, unsigned char SamplingTime, unsigned int SamplesInAverage);


//------------------------------------------------------------------------------
//Public defintions
//------------------------------------------------------------------------------
//Definitions for the ADCReferences parameter
#define POSREF_AVDD     0x0000
#define POSREF_EXTREF   0x4000
#define POSREF_BGBUF    0x8000

#define NEGREF_AVSS     0x0000
#define NEGREF_EXTREF   0x1000

//Definitions for the ADCFormat parameter
#define ADCFORM_UINT        0x0000
#define ADCFORM_SINT        0x0100
#define ADCFORM_SFRACTIONAL 0x0300
#define ADCFORM_UFRACTIONAL 0x0200


#endif //__ADC_PIPELINE__H

//*******************************
//EOF ADC_Pipeline.h

