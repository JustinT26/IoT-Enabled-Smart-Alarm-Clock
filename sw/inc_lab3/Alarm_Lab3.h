/* Alarm_Lab3.h
   Justin Thai
   Walter Contreras
   09/20/2025
*/

#ifndef ALARM_LAB3_H
#define ALARM_LAB3_H


#include <stdint.h>
#include <string.h>
#include <stdio.h>


/* length for string representation of alarm */
#define     ALARM_STRING_SIZE   9  // "hh:mm AM" or "hh:mm PM"


/* initialize alarm to be none */
void Alarm_Init(void);


/* set alarm based on a input string */
void Alarm_Set(char* newAlarmString);


/* raise AlarmPlaying flag */
void Alarm_Enable(void);


/* lower AlarmPlaying flag */
void Alarm_Disable(void);


/* check and enable alarm if time is met */
void Alarm_Check(void);


/* stop playing alarm */
void Alarm_Stop(void);


/* reset alarm to none */
void Alarm_Reset(void);


/* get current alarm as string in format "hh:mm AM" or "hh:mm PM" */
char* getAlarmTimeString(void);

#endif