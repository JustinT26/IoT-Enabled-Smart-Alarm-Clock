/* SwitchInterface_Lab3.c
   Justin Thai
   Walter Contreras
   09/16/2025
*/

#include "SwitchInterface_Lab3.h"


/* initialize PC5, PB4, PE3 as input switches */
void Switch_Init(void)
{
	volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= (PORT_C_MASK | PORT_B_MASK | PORT_E_MASK);  		// activate clock for Port B, C, E
  delay = SYSCTL_RCGCGPIO_R;  			                    // allow time for clock to start

  //initialize PC5 as input
  GPIO_PORTC_DIR_R &= ~PIN_5_MASK;  		                // set pin 5 direction input
  GPIO_PORTC_DEN_R |= PIN_5_MASK;   		                // enable PC5 digital port

  //initialize PB4 as input
  GPIO_PORTB_DIR_R &= ~PIN_4_MASK;                      // set direction PB4 input     
  GPIO_PORTB_DEN_R |= PIN_4_MASK;                       // enable PB4 digital port 
  
  //initialize PE3 as input
  GPIO_PORTE_DIR_R &= ~PIN_3_MASK;                      // set direction PE3 input     
  GPIO_PORTE_DEN_R |= PIN_3_MASK;                       // enable PE3 digital port 

  //initialize PB3 as input
  GPIO_PORTB_DIR_R &= ~PIN_3_MASK;                      // set direction PB3 input     
  GPIO_PORTB_DEN_R |= PIN_3_MASK;                       // enable PB3 digital port 

  
  //initialize PB1 as input
  GPIO_PORTB_DIR_R &= ~PIN_1_MASK;                      // set direction PB1 input     
  GPIO_PORTB_DEN_R |= PIN_1_MASK;                       // enable PB1 digital port 
  
  Switch_Interrupt_Init();                              // initialize GPIO interrupts for PC5 and PB4
}


/* read PC5, return 1 (pressed) or 0 (not pressed) */
uint8_t PC5_Switch_In(void){
  return (GPIO_PORTC_DATA_R & PIN_5_MASK) >> 5;
}


/* read PB4, return 1 (pressed) or 0 (not pressed) */
uint8_t PB4_Switch_In(void){
  return (GPIO_PORTB_DATA_R & PIN_4_MASK) >> 4;
}


/* read PE3, return 1 (pressed) or 0 (not pressed) */
uint8_t PE3_Switch_In(void) {
  return (GPIO_PORTE_DATA_R & PIN_3_MASK) >> 3;
}

/* read PB3, return 1 (pressed) or 0 (not pressed) */
uint8_t PB3_Switch_In(void) {
  return (GPIO_PORTB_DATA_R & PIN_3_MASK) >> 3;
}

uint8_t PB1_Switch_In(void){
  return (GPIO_PORTB_DATA_R & PIN_1_MASK) >> 1;
}


/* initialize GPIO interrupts for PC5, PB4, PE3 */
void Switch_Interrupt_Init(void)
{
  /*  PC5 Interrupt Setup */
  GPIO_PORTC_IS_R  &= ~PIN_5_MASK;                    // PC5 is edge sensitive   
  GPIO_PORTC_IBE_R &= ~PIN_5_MASK;                    // PC5 is not both edges
  GPIO_PORTC_IEV_R |= PIN_5_MASK;                     // PC5 is rising edge event
  GPIO_PORTC_ICR_R = PIN_5_MASK;                      // clear flag5 
  GPIO_PORTC_IM_R  |= PIN_5_MASK;                     // arm interrupt on PC5
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF)|0x00A00000;  // priority 5
  NVIC_EN0_R |= 0x04;                                 // enable interrupt 2 (port C) in NVIC  

  /*  PB4 Interrupt Setup */
  GPIO_PORTB_IS_R  &= ~PIN_4_MASK;                    // PB4 is edge sensitive
  GPIO_PORTB_IBE_R &= ~PIN_4_MASK;                    // PB4 is not both edges
  GPIO_PORTB_IEV_R |= PIN_4_MASK;                     // PB4 is rising edge event
  GPIO_PORTB_ICR_R = PIN_4_MASK;                      // clear flag4
  GPIO_PORTB_IM_R  |= PIN_4_MASK;                     // arm interrupt on PB4
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF)|0x0000A000;  // priority 5
  NVIC_EN0_R |= 0x02;                                 // enable interrupt 1 (port B) in NVIC

  /*  PE3 Interrupt Setup */
  GPIO_PORTE_IS_R  &= ~PIN_3_MASK;                     // PE3 is edge sensitive
  GPIO_PORTE_IBE_R &= ~PIN_3_MASK;                     // PE3 is not both edges
  GPIO_PORTE_IEV_R |= PIN_3_MASK;                      // PE3 is rising edge event
  GPIO_PORTE_ICR_R = PIN_3_MASK;                       // clear flag3
  GPIO_PORTE_IM_R  |= PIN_3_MASK;                      // arm interrupt on PE3
  NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFFFF1F) | 0x000000A0;  // priority 5
  NVIC_EN0_R |= 0x10;                                  // enable interrupt 4 (port E) in NVIC

  /* PB3 Interrupt Setup */
  GPIO_PORTB_IS_R  &= ~PIN_3_MASK;                     // PB3 is edge sensitive
  GPIO_PORTB_IBE_R &= ~PIN_3_MASK;                     // PB3 is not both edges
  GPIO_PORTB_IEV_R |= PIN_3_MASK;                      // PB3 is rising edge event
  GPIO_PORTB_ICR_R = PIN_3_MASK;                       // clear flag3
  GPIO_PORTB_IM_R  |= PIN_3_MASK;                      // arm interrupt on PB3
  //NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF00FF) | 0x0000A000;  // priority 5
  NVIC_PRI0_R = (NVIC_PRI0_R & ~0x0000E000) | 0x0000A000;
  NVIC_EN0_R |= 0x02;                                  // enable interrupt 1 (port B) in NVIC

  /* PB1 Interrupt Setup */
  GPIO_PORTB_IS_R  &= ~PIN_1_MASK;                     // PB1 is edge sensitive
  GPIO_PORTB_IBE_R &= ~PIN_1_MASK;                     // PB1 is not both edges
  GPIO_PORTB_IEV_R |= PIN_1_MASK;                      // PB1 is rising edge event
  GPIO_PORTB_ICR_R = PIN_1_MASK;                       // clear flag1
  GPIO_PORTB_IM_R  |= PIN_1_MASK;                      // arm interrupt on PB1
  //NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF00FF) | 0x0000A000;  // priority 5
  NVIC_PRI0_R = (NVIC_PRI0_R & ~0x0000E000) | 0x0000A000;
  NVIC_EN0_R |= 0x02;                                  // enable interrupt 1 (port B) in NVIC
}


// -----------Clock Delay for Debouncing-----------
// ------------Clock_Delay1ms------------
// Simple delay function which delays about n milliseconds.
// Inputs: n, number of msec to wait
// Outputs: none
void Clock_Delay1ms(volatile uint32_t n){
  while(n){
    Clock_Delay(23746);  // 1 msec, tuned at 80 MHz, originally part of LCD module
    n--;
  }
}

void Clock_Delay(volatile uint32_t ulCount){
  while(ulCount){
    ulCount--;
  }
}