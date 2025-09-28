/* SwitchInterface_Lab3.h
   Justin Thai
   Walter Contreras
   09/16/2025
*/

#ifndef SWITCHINTERFACE_LAB3_H
#define SWITCHINTERFACE_LAB3_H


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>


/* Switch1   ->  PC5 */
#define		PORT_C_MASK			0x04
#define		PIN_5_MASK 		 	0x20


/* Switch2  ->  PB4 */
#define     PORT_B_MASK       0x02
#define     PIN_4_MASK        0x10


/* Switch3 -> PE3 */
#define     PORT_E_MASK       0x10
#define     PIN_3_MASK        0x08


/* Switch4 -> PB3 */


/* Switch5 -> PB1 */
#define     PIN_1_MASK         0x02


/* initialize PC5, PB4, PE3 as input switches */
void Switch_Init(void);


/*  read PC5, return 1 (pressed) or 0 (not pressed) */
uint8_t PC5_Switch_In(void);


/* read PB4, return 1 (pressed) or 0 (not pressed) */
uint8_t PB4_Switch_In(void);


/* read PE3, return 1 (pressed) or 0 (not pressed) */
uint8_t PE3_Switch_In(void);


/* read PB3, return 1 (pressed) or 0 (not pressed) */
uint8_t PB3_Switch_In(void);


/* read PB1, return 1 (pressed) or 0 (not pressed) */
uint8_t PB1_Switch_In(void);


/* initialize GPIO interrupts for PC5, PB4, PE3 */
void Switch_Interrupt_Init(void);


// Clock delay for debouncing
void Clock_Delay1ms(volatile uint32_t n);    // delay for n milliseconds
void Clock_Delay(volatile uint32_t ulCount);

#endif