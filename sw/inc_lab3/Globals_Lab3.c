/* Globals_Lab3.c
   Justin Thai
   Walter Contreras
   09/16/2025
*/

#include "Globals_Lab3.h"


/* Flags for switches */
volatile uint8_t PC5_Pressed = FALSE;
volatile uint8_t PB4_Pressed = FALSE;
volatile uint8_t PE3_Pressed = FALSE;
volatile uint8_t PB3_Pressed = FALSE;
volatile uint8_t PB1_Pressed = FALSE;


/* inactivity counter */
volatile uint32_t inactivityCounter = 0; 


/* Time Digit Position */
volatile uint8_t pos = 0;   // 12 : 10    time
                            // 32   10    digit position


/* Flags for what time needs to be set (clock or alarm) */
volatile uint8_t needToSetClock = 0;
volatile uint8_t needToSetAlarm = 0;


/* Cursor Position */
volatile uint8_t currentOption = 0;


/* FSM State */
uint8_t currentState = 0;


/* Time */
volatile uint8_t TimeHours   = 0;
volatile uint8_t TimeMinutes = 0;
volatile uint8_t TimeSeconds = 0;
volatile uint8_t AM_or_PM = 1; // 0 for AM, 1 for PM


/* Alarm */
volatile int8_t AlarmHours   = 0;
volatile int8_t AlarmMinutes = 0;
volatile uint8_t Alarm_AM_PM  = AM;
volatile uint8_t AlarmPlaying  = FALSE;  
volatile uint8_t isAlarmSet = FALSE;


/* 24 hour mode */
volatile uint8_t is24hourMode = FALSE;


/* Dancing Guy Flag */ 
volatile uint8_t isGuyDancing = FALSE;

/*Website Flag*/
volatile uint8_t OnWebsite = FALSE; 


/* toggles 24 hour mode */
void toggle24HourMode(void){
    is24hourMode = !is24hourMode;

    if(is24hourMode){
        // Clock
        if((AM_or_PM == AM) && TimeHours == 12) { 
            TimeHours = 0;
        } else if((AM_or_PM == PM) && TimeHours < 12) {
            TimeHours += 12;
        }

        // Alarm
        if((Alarm_AM_PM == AM) && AlarmHours == 12){
            AlarmHours = 0;
        } else if((Alarm_AM_PM == PM) && AlarmHours < 12){
            AlarmHours += 12;
        }
    }
    else {
        // Clock
        if(TimeHours == 0){
            TimeHours = 12;
            AM_or_PM = AM;
        } else if(TimeHours == 12){
            AM_or_PM = PM;
        } else if(TimeHours > 12){
            TimeHours -= 12;
            AM_or_PM = PM;
        } else {
            AM_or_PM = AM;
        }

        // Alarm
        if(AlarmHours == 0){
            AlarmHours = 12;
            Alarm_AM_PM = AM;
        } else if(AlarmHours == 12){
            Alarm_AM_PM = PM;
        } else if(AlarmHours > 12){
            AlarmHours -= 12;
            Alarm_AM_PM = PM;
        } else {
            Alarm_AM_PM = AM;
        }
    }
}