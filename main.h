/* 
 * File:   main.h
 * Author: micma
 *
 * Created on December 9, 2020, 10:12 AM
 */

#ifndef MAIN_H
#define	MAIN_H

void check_Tastendruck_MM (void);
void Value_to_SEG7 (uint16_t value);
/* ACHTUNG BEI ÄNDERUNG AUCH KOMMA ÄNDER IN TMR0 ISR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
volatile const uint8_t SEG7_map [10] = {0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09}; // Zahlen 0-9 commen Anode
volatile const uint8_t SEG7_segm_select [4] = {0x10, 0x20, 0x40, 0x80};                              // Segmentauswahl 1-4 commen Anode
*/
volatile const uint8_t SEG7_map [15] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6, 0x40,0x04,0x02,0x1C,0x00}; // Zahlen 0-9 LED V,C,P commen Kathode
volatile const uint8_t SEG7_segm_select [4] = {0xEF, 0xDF, 0xBF, 0x7F};                              // Segmentauswahl 1-4 commen Kathode

volatile const uint8_t glaettwert=4;
volatile uint8_t SEG7_NR_Value[4];
volatile uint8_t komma, OL;
volatile int SEG7_to_write=4;

uint32_t ADC_raw_current_10000, ADC_raw_current_100=0;
uint32_t ADC_raw_voltage_10000, ADC_raw_voltage_100=0;
//volatile uint16_t ADC_raw_current=0, ADC_raw_voltage=0;
uint32_t current, voltage;
uint32_t power;
volatile uint8_t show_val=0;
volatile uint8_t TMR1_overflow_MM;

#endif	/* MAIN_H */

