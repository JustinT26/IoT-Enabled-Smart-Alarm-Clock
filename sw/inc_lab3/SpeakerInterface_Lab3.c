/* SpeakerInterface_Lab3.c
   Justin Thai
   Walter Contreras
   09/16/2025
*/

#include "SpeakerInterface_Lab3.h"


volatile uint8_t isSpeakerOn = 0;


/* initialize PC7 as output pin for speaker */
void Speaker_Init(void){
    volatile unsigned long delay;
    SYSCTL_RCGCGPIO_R |= PORT_C_MASK;           // activate clock for Port C
    delay = SYSCTL_RCGCGPIO_R;                  // allow time for clock to start
    GPIO_PORTC_DIR_R |= PIN_7_MASK;             // set pin 7 direction to output   
    GPIO_PORTC_DEN_R |= PIN_7_MASK;             //  enable digital port
    GPIO_PORTC_DR2R_R |= PIN_7_MASK;            // 2mA drive
}


/* start SysTick 1000 Hz interrupt -> turns on speaker output */
void Speaker_On(void){
    NVIC_ST_CTRL_R = 0;                  // disable SysTick during setup
    NVIC_ST_RELOAD_R = 40000-1;          // 80000 Hz / 40,000 = 2000 Hz
                                         // toggling -> 1000 Hz square wave
    NVIC_ST_CURRENT_R = 0;               // clear current
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF); // priority 0
    NVIC_ST_CTRL_R = 0x07;               // enable SysTick w/ core clock + interrupt
    isSpeakerOn = 1;
}


/* disable SysTick intterupt -> turns off speaker output */
void Speaker_Off(void){
    NVIC_ST_CTRL_R = 0;                  // disable SysTick
    GPIO_PORTC_DATA_R &= ~PIN_7_MASK;    // force PC7 low (speaker quiet)
    isSpeakerOn = 0;
}


/* check if speaker is playing sound*/
uint8_t getIsSpeakerOn(void) {
    return isSpeakerOn;
}


/* toggles speaker output pin PC7 */
volatile uint8_t pwmCounter = 0;
void SysTick_Handler(void){
    pwmCounter++;
    if(pwmCounter < VOLUME){  
        GPIO_PORTC_DATA_R ^= PIN_7_MASK;   // toggle PC7 (normal tone)
    } 
    else {
        GPIO_PORTC_DATA_R &= ~PIN_7_MASK;  // hold low (quiet portion)
    }
    if(pwmCounter >= 2*VOLUME){  
        pwmCounter = 0;   // reset after full cycle
    }
}