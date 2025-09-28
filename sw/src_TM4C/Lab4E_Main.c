/* Lab4E_Main.c
   Justin Thai
   Walter Contreras
   09/24/2025
*/


#include <stdio.h>
#include <stdint.h>

#include "inc/tm4c123gh6pm.h"
#include "inc/ST7735.h"
#include "inc/PLL.h"
#include "inc/Timer2A.h"
#include "inc/Timer5A.h"
#include "inc/UART.h"
#include "inc/UART5.h"
#include "inc/esp8266_base.h"
#include "inc/MQTT.h"
#include "inc/Unified_Port_Init.h"

/* Lab 3 imports */
#include "../inc_lab3/SwitchInterface_Lab3.h"
#include "../inc_lab3/SpeakerInterface_Lab3.h"
#include "../inc_lab3/UpdateDisplay_Lab3.h"
#include "../inc_lab3/FSM_Lab3.h"
#include "../inc_lab3/ManageTime_Lab3.h"
#include "../inc_lab3/Globals_Lab3.h"
#include "../inc_lab3/Alarm_Lab3.h"
#include "../inc_lab3/ADC_Lab3.h"


/* Hardware Connection Specifications
    - PC5 is OK/Select
    - PB4 is to move cursor
    - PE3 is to toggle darkmode
    - PB3 is to make a guy dance
    - PB1 is to toggle 24 hour mode
    - PC7 is squarewave output to speaker
      - if alarm is sounding, any button (PC5, PB4, PE3) will quiet the alarm
    - PD3 is ADC input from IR sensor
      - if alarm is sounding, hovering over the IR sensor will quiet the alarm
*/


//----- Prototypes of functions in startup.s  ----------------------
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts


// -----------------------------------------------------------------
// -------------------- MAIN LOOP ----------------------------------
//
int main(void){  
  DisableInterrupts();           
  
  PLL_Init(Bus80MHz);             
  Port_E_Init();                  // initialize ESP8266 connections to Port E
  UART_Init();                    // PC communication
  UART5_Init();                   // ESP8266 communication

  Switch_Init();                  // initialize switch buttons PC5, PB4, PE3, PB3, PB1
  Speaker_Init();                 // initialize speaker output to PC7
  ST7735_InitR(0);                // initialize ST7735 LCD screen
  ST7735_SetRotation(2); 
  ManageTime_Init(5, 1, PM, 3);   // initialize starting time for the alarm clock 
  Alarm_Init();                   // initialize alarm system
  ADC_Init();                     // initialize ADC sampling for external alarm-silencing IR sensor
  
  Reset_8266();                   // Reset ESP8266 WiFi module
  SetupWiFi();                    // Setup communications to MQTT Broker via 8266 WiFi
  
  Timer2A_Init(&MQTT_to_TM4C, 400000, 7);         // Check/Get data from the ESP every 5ms 
  Timer5A_Init(&TM4C_to_MQTT, 80000000, 7);       // Send data back to MQTT Web App every second 
  
  EnableInterrupts();
  
  currentState = mainMenu_setAlarmOption;      
  DrawMainMenu(); 
   while(1){
    uint8_t input = 0;

    // Build input index from PC5/PB4/PE3/PB3/PB1 flags
    if(PB4_Pressed){
      Clock_Delay1ms(10);          // debounce 
      if(PB4_Switch_In()){         // confirm it’s still pressed
        input = 1;             // valid press
      }
      PB4_Pressed = FALSE;         // clear flag
    }
    else if(PC5_Pressed){
      Clock_Delay1ms(10);          // debounce 
      if(PC5_Switch_In()){         // confirm it’s still pressed
        input = 2;             // valid press
      }
      PC5_Pressed = FALSE;         // clear flag
    }
    else if(PE3_Pressed){
      Clock_Delay1ms(10);          // debounce 
      if(PE3_Switch_In()){         // confirm it’s still pressed
        input = 4;                // valid press
      }
      PE3_Pressed = FALSE;         // clear flag
    }
    else if(PB3_Pressed){
      Clock_Delay1ms(10);          // debounce 
      if(PB3_Switch_In()){         // confirm it’s still pressed
        input = 8;                // valid press
      }
      PB3_Pressed = FALSE;         // clear flag
    }
    else if(PB1_Pressed){
      Clock_Delay1ms(10);
      if(PB1_Switch_In()){
        input = 16;
      }
      PB1_Pressed = FALSE;
    }

    //check if user has been inactive for too long
    if(inactivityCounter >= INACTIVITY_MAX_SEC) {
      currentState = mainMenu_setAlarmOption;
      UpdateDisplay(currentState);
      isGuyDancing = FALSE; //stop showing dancing guy after inactivity
      inactivityCounter = 0;
      pos = 0;
    }

    // if user pressed a button, change state and update display
    if(input != 0)
    {
      inactivityCounter = 0; //user pressed a button, reset inactivity counter
      if(AlarmPlaying){
        Alarm_Stop(); 
        continue; // stop alarm, dont change state
      }

      if(input == 4){ // PE3 pressed
        toggleDarkMode(); 
        continue; // toggle dark mode, dont change state
      }

      if(input == 8) { // PB3 pressed
        isGuyDancing = TRUE;
        DrawDanceMeme();
        continue; //dont change state
      }

      if(input == 16) { // PB1 pressed
         toggle24HourMode();
         continue;
      }
      
      if(currentState == mainMenu_setTimeOption) {
        if(input == 2) { // PC5 (ok) pressed
          needToSetClock = TRUE; 
        }
      }
      else if(currentState == alarmMenu_cancelAlarmOption) {
        if(input == 2) {
          Alarm_Reset(); // canacel alarm
        }
      }
      else if (currentState == alarmMenu_setAlarmOption) {
        if(input == 2) { // PC5 (ok) pressed
          needToSetAlarm = TRUE;
        }
      }
      else if(currentState == timeMenu_increaseOption) {
        if(input == 2) {
          if(pos == 1 || pos == 2 || pos == 3 || pos == 4) {
            Alarm_Or_Time_IncrementDigit();
          }
          else if(pos == 0) {
            TimeMenu_ToggleAM_PM();
          }
          continue;  // skip state update so cursor doesn’t move
        }
      }
      else if(currentState == timeMenu_okOption) {
        if(input == 2) { // PC5 (ok) pressed
          TimeMenu_MoveArrowLeft(); // move arrow left after clicking ok
          pos++; // move to next digit
          if(pos > 4) 
          {
            pos = 0; //wrap around after last digit
            currentState = mainMenu_setAlarmOption; //return to main menu
          }
          else {
            currentState = timeMenu_increaseOption; // go back to increase option for next digit
          }
          UpdateDisplay(currentState); 
          continue;
        }
      }
      currentState = FSM[currentState].next[input];
      UpdateDisplay(currentState);
    }

    // check ADC
    if(Num >= BUFSIZE){               // once buffer is full
      Signal = CalculateSignal();              // compute average signal 
      Distance = IR_Convert(Signal);  // use averaged signal
      Num = 0;                        // reset for next batch
    }
    if((Distance/10) < DISTANCE_THRESHOLD_CM){
      if(AlarmPlaying){
        Alarm_Stop();
      }
      if(getIsSpeakerOn()){  // if speaker is playing but not because of the alarm going off
        Speaker_Off();
      }
    }
  }
}


// GPIO PortC interrupt handler
void GPIOPortC_Handler(void){                              
  if(GPIO_PORTC_RIS_R & (PIN_5_MASK)){  // PC5
    GPIO_PORTC_ICR_R = PIN_5_MASK;      // acknowledge interrupt
    PC5_Pressed = TRUE;                 // set PC5 flag
  }
}


// GPIO PortB interrupt handler
void GPIOPortB_Handler(void){
  if(GPIO_PORTB_RIS_R & (PIN_4_MASK)){  // PB4
    GPIO_PORTB_ICR_R = PIN_4_MASK;      // acknowledge interrupt
    PB4_Pressed = TRUE;                 // set PB4 flag
  }  
  
  if(GPIO_PORTB_RIS_R & (PIN_3_MASK)){  // PB3
    GPIO_PORTB_ICR_R = PIN_3_MASK;      // acknowledge interrupt
    PB3_Pressed = TRUE;                 // set PB3 flag
  } 
  
  if(GPIO_PORTB_RIS_R & (PIN_1_MASK)){  // PB1
    GPIO_PORTB_ICR_R = PIN_1_MASK;      // acknowledge interrupt
    PB1_Pressed = TRUE;                 // set PB1 flag
  }  
}


// GPIO PortE interrupt handler
void GPIOPortE_Handler(void){
  if(GPIO_PORTE_RIS_R & (PIN_3_MASK)){  // PE3
    GPIO_PORTE_ICR_R = PIN_3_MASK;      // acknowledge interrupt
    PE3_Pressed = TRUE;                 // set PE3 flag
  }                                   
}


void moveToNextOption() {
  currentOption++;
  if(currentOption > 2) {
    currentOption = 0;
  }
}