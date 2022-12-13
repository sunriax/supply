/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC12F1572
        Driver Version    :  2.00
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

#include <stdint.h>
#include "main.h"
#include "mcc_generated_files/mcc.h"

/*
                         Main application
 */
void main(void){
    uint32_t testwert=1024;
    TMR1_overflow_MM=0;
    // initialize the device
    SYSTEM_Initialize();
    EUSART_Initialize();
    LATCH_SetDigitalOutput();
    LATCH_SetPushPull(); 
    LATCH_SetLow();                  
    
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Disable the Global Interrupts
    INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptDisable();
    EUSART_Initialize();
    __delay_ms(100);
    
        // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
     
    TMR1_StartTimer();
    TMR1_StartSinglePulseAcquisition();
    show_val = 0;
    int conversions=0;
    
    while (1)
    {
        // Add your application code
        
        ADC_raw_current_10000 = (glaettwert * (uint32_t)ADC1_GetConversion(AN2_Current)*100 + (100-glaettwert)*ADC_raw_current_100);     //AN2_Current =  0x2
        //ADC_raw_voltage = ((uint32_t)glaettwert * ADC1_GetConversion(AN3_Voltage) + (uint32_t)(100-glaettwert)*ADC_raw_voltage)/100;     // AN3_Voltage =  0x3
        //ADC_raw_current_10000 = (glaettwert * (testwert*100) + (100-glaettwert)*(ADC_raw_current_100));     //AN2_Current =  0x2
        ADC_raw_current_100=ADC_raw_current_10000/100;
        
        ADC_raw_voltage_10000 = (glaettwert * (uint32_t)ADC1_GetConversion(AN3_Voltage)*100 + (100-glaettwert)*ADC_raw_voltage_100); 
        ADC_raw_voltage_100=ADC_raw_voltage_10000/100;
        
        current = ((ADC_raw_current_100/100)  *  20 +      0)  *  19000/19000 + 120  ; // 0-5V Skaliert auf 2A plus Abgleich und offset korrekur
                                       // Skalierung  grundoffset  Abgleich  offset
        current = current -(5000/current); // Offset bis 100mA abziehen
        
        if (current >= 100000){ //offset under-overflow abschneiden
            current = 0;
        }   
       // Value_to_SEG7 (current);
//MM TODO FEHLT Voltage Komma einfügen //        
        voltage = (ADC_raw_voltage_100/100) *  3      *2900/2900   -    (current / 1277)                     +          3;   // Skaliert auf 30V + Abgleich
                                        // Skalierung   Abgleich   Current_Offset = currentv / (100/Shunt)        grundoffset
         if (voltage >= 100000){ //offset under-overflow abschneiden
            voltage = 0;
        }
        //power = (ADC_raw_voltage_100/100) * (ADC_raw_current_100/100)/1046*2; // Skaliert auf 20W
       
        
     //   voltage = voltage /10 *10; // 4te Stelle Gestrichen
     //   current = current /10 *10; // 4te Stelle Gestrichen
        
      
        power = current * voltage / 10000;
       
        if (voltage > 3050 || current > 20300){ 
            
            OL=1;
        }
        
        else {
            OL=0;
        }
        
        if (conversions > 100) {
                      
            if (OL==1){
             SEG7_NR_Value[0]=14; //LED V-C-P off //__delay_ms(200);
            }
            if (conversions > 200){
                conversions = 0;
            }
            
        }
        
        if(show_val == 0){  // Spannungsanzeige
        //Value_to_SEG7 (TMR1_overflow_MM*10);   
        
            if (conversions<100) {
            
                SEG7_NR_Value[0]=10; //LED Voltage on
            }
            if (voltage > 3050) {
                       
               SEG7_NR_Value[3]=0;
               SEG7_NR_Value[2]=13;
               SEG7_NR_Value[1]=14;
               komma=0;                
            } 
            else {
            
                komma = 2;
                Value_to_SEG7 (voltage);
            }
        }
        
       if(show_val == 1){ // Stromanzeige
           
           if (conversions<100) SEG7_NR_Value[0]=11;  //LED Current on
           
           if (current > 20300) {
               
               SEG7_NR_Value[3]=0;
               SEG7_NR_Value[2]=13;
               SEG7_NR_Value[1]=14;
               komma=0;          
            } 
            else {
                        
                if(current >= 0*9990){
                    current=current/10;
                    komma=3;
            
                }else komma=0;
        
                Value_to_SEG7 (current);//current);
                               
            }     
       
        //ADC1_GetConversion(AN2_Current));//current);
  
       }
        
       if(show_val == 2){  // Leistungsanzeige
        
           if (conversions<100) SEG7_NR_Value[0]=12;  //LED Power
           
           Value_to_SEG7 (power);
        
        komma=2;
       }
      
       TMR1_StartSinglePulseAcquisition(); 
       conversions=conversions+1;
       // __delay_ms(2);
    }
}
void Value_to_SEG7 (uint16_t value){

	SEG7_NR_Value[3]= value/1000;
  	SEG7_NR_Value[2]=(value/100)%10;
	SEG7_NR_Value[1]=(value/10)%10;
}



/**
 End of File
*/