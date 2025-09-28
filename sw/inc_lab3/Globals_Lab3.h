/* Globals_Lab3.h
   Justin Thai
   Walter Contreras
   09/16/2025
*/

#ifndef GLOBALS_LAB3_H
#define GLOBALS_LAB3_H


#include <stdint.h>
#include "UpdateDisplay_Lab3.h"


#define   TRUE    1
#define   FALSE   0


enum {AM, PM};


/* Flags for switches */
extern volatile uint8_t PC5_Pressed;
extern volatile uint8_t PB4_Pressed;
extern volatile uint8_t PE3_Pressed;
extern volatile uint8_t PB3_Pressed;
extern volatile uint8_t PB1_Pressed;


/* inactivity counter */
extern volatile uint32_t inactivityCounter;


/* Time Digit Position */
extern volatile uint8_t pos;   // 12 : 10    time
                               // 32   10    digit position


/* Flags for what time needs to be set (clock or alarm) */
extern volatile uint8_t needToSetClock;
extern volatile uint8_t needToSetAlarm;


/* Cursor Position */
extern volatile uint8_t currentOption;


/* FSM State */
extern uint8_t currentState;


/* Time */
extern volatile uint8_t TimeHours;      // 0..12
extern volatile uint8_t TimeMinutes;    // 0..59
extern volatile uint8_t TimeSeconds;    // 0..59
extern volatile uint8_t AM_or_PM;


/* Alarm */
extern volatile int8_t AlarmHours;     // 0..12
extern volatile int8_t AlarmMinutes;   // 0..59
extern volatile uint8_t Alarm_AM_PM;    // 0..59
extern volatile uint8_t AlarmPlaying;
extern volatile uint8_t isAlarmSet;


/* 24 hour mode */
extern volatile uint8_t is24hourMode;


/* Dancing Guy Flag */ 
extern volatile uint8_t isGuyDancing;

/* Webside Flag*/
extern volatile uint8_t OnWebsite;

/* toggles 24 hour mode */
void toggle24HourMode(void);

#endif 