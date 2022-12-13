/**
  TMR1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr1.c

  @Summary
    This is the generated driver implementation file for the TMR1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for TMR1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC12F1572
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.30 and above
        MPLAB 	          :  MPLAB X 5.40
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "tmr1.h"
#include "../main.h"
/**
  Section: Global Variables Definitions
*/
volatile uint16_t timer1ReloadVal;
void (*TMR1_InterruptHandler)(void);
void (*TMR1_GateInterruptHandler)(void);
static void TMR1_DefaultGateInterruptHandler(void);

/**
  Section: TMR1 APIs
*/

void TMR1_Initialize(void)
{
    //Set the Timer to the options selected in the GUI

    //T1GSS T1G_pin; TMR1GE enabled; T1GTM enabled; T1GPOL high; T1GGO ready; T1GSPM enabled; 
    T1GCON = 0xF8;

    //TMR1H 236; 
    TMR1H = 0xEC;

    //TMR1L 120; 
    TMR1L = 0x78;

    // Clearing IF flag before enabling the interrupt.
    PIR1bits.TMR1IF = 0;

    // Load the TMR value to reload variable
    timer1ReloadVal=(uint16_t)((TMR1H << 8) | TMR1L);

    // Enabling TMR1 interrupt.
    PIE1bits.TMR1IE = 1;

    // Set Default Interrupt Handler
    TMR1_SetInterruptHandler(TMR1_DefaultInterruptHandler);

    // Clearing IF flag before enabling the interrupt.
    PIR1bits.TMR1GIF = 0;

    // Enabling TMR1 interrupt.
    PIE1bits.TMR1GIE = 1;

    // Set Default Gated Interrupt Handler
    TMR1_SetGateInterruptHandler(TMR1_DefaultGateInterruptHandler);

    // T1CKPS 1:8; nT1SYNC do_not_synchronize; TMR1CS FOSC/4; TMR1ON enabled; 
    T1CON = 0x35;
}

void TMR1_StartTimer(void)
{
    // Start the Timer by writing to TMRxON bit
    T1CONbits.TMR1ON = 1;
}

void TMR1_StopTimer(void)
{
    // Stop the Timer by writing to TMRxON bit
    T1CONbits.TMR1ON = 0;
}

uint16_t TMR1_ReadTimer(void)
{
    uint16_t readVal;
    uint8_t readValHigh;
    uint8_t readValLow;
    
	
    readValLow = TMR1L;
    readValHigh = TMR1H;
    
    readVal = ((uint16_t)readValHigh << 8) | readValLow;

    return readVal;
}

void TMR1_WriteTimer(uint16_t timerVal)
{
    if (T1CONbits.nT1SYNC == 1)
    {
        // Stop the Timer by writing to TMRxON bit
        T1CONbits.TMR1ON = 0;

        // Write to the Timer1 register
        TMR1H = (timerVal >> 8);
        TMR1L = timerVal;

        // Start the Timer after writing to the register
        T1CONbits.TMR1ON =1;
    }
    else
    {
        // Write to the Timer1 register
        TMR1H = (timerVal >> 8);
        TMR1L = timerVal;
    }
}

void TMR1_Reload(void)
{
    TMR1_WriteTimer(timer1ReloadVal);
}

void TMR1_StartSinglePulseAcquisition(void)
{
    T1GCONbits.T1GGO = 1;
}

uint8_t TMR1_CheckGateValueStatus(void)
{
    return (T1GCONbits.T1GVAL);
}

void TMR1_ISR(void)
{

    // Clear the TMR1 interrupt flag
    PIR1bits.TMR1IF = 0;
    TMR1_WriteTimer(timer1ReloadVal);

    if(TMR1_InterruptHandler)
    {
        TMR1_InterruptHandler();
    }
    /****MMstart */
    
    //TMR1_overflow_MM++;
    
    /*MMend *****/
}


void TMR1_SetInterruptHandler(void (* InterruptHandler)(void)){
    TMR1_InterruptHandler = InterruptHandler;
}

void TMR1_DefaultInterruptHandler(void){
    // add your TMR1 interrupt custom code
    
    // or set custom function using TMR1_SetInterruptHandler()
}

void TMR1_GATE_ISR(void)
{
    // clear the TMR1 interrupt flag
    PIR1bits.TMR1GIF = 0;

    if(TMR1_GateInterruptHandler)
    {
        TMR1_GateInterruptHandler();
    }
   
    /****MMstart  */ 
    TMR1_overflow_MM++;
    //TMR1_StartSinglePulseAcquisition();
    show_val++;
    if(show_val==3) show_val=0;
    TMR1_overflow_MM++;
    
    /* MMend *****/
}

void TMR1_SetGateInterruptHandler(void (* InterruptHandler)(void)){
    TMR1_GateInterruptHandler = InterruptHandler;
}

static void TMR1_DefaultGateInterruptHandler(void){
    // add your TMR1 Gate interrupt custom code
    // or set custom function using TMR1_SetGateInterruptHandler()
    
}
/**
  End of File
*/
