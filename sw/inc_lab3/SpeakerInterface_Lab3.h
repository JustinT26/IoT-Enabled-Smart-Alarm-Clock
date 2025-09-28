/* SpeakerInterface.h
   Justin Thai
   Walter Contreras
   09/16/2025
*/

#ifndef SPEAKERINTERFACE_LAB3_H
#define SPEAKERINTERFACE_LAB3_H


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>


#define     PORT_C_MASK         0x04
#define     PIN_7_MASK          0x80


// volume control: 1 = loudest, higher numbers = quieter
#define     VOLUME              15


/*flag to see if speaker is on*/
extern volatile uint8_t isSpeakerOn;


/* initialize PC7 as output pin for speaker */
void Speaker_Init();


/* start SysTick 1000 Hz interrupt -> turns on speaker output */
void Speaker_On(void);


/* disable SysTick intterupt -> turns off speaker output */
void Speaker_Off(void);


/* check if speaker is playing sound*/
uint8_t getIsSpeakerOn(void);

#endif