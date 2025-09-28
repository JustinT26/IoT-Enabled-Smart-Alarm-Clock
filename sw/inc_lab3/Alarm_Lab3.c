/* Alarm_Lab3.c
   Justin Thai
   Walter Contreras
   09/20/2025
*/

#include "Alarm_Lab3.h"
#include "Globals_Lab3.h"
#include "SpeakerInterface_Lab3.h"


/* initialize alarm to be none */
void Alarm_Init(void) {
  Alarm_Reset();
  Alarm_Disable();
}


/* set alarm based on a input string 
   prereq: newAlarmString format expected: "hh:mm AM" or "hh:mm PM"
*/
void Alarm_Set(char* newAlarmString){
  isAlarmSet = TRUE;
  AlarmHours = ((newAlarmString[0] - '0') * 10) + (newAlarmString[1] - '0');
  AlarmMinutes = ((newAlarmString[3] - '0') * 10) + (newAlarmString[4] - '0');

   if(is24hourMode){
    // In 24-hour mode, no AM/PM in string
    if(AlarmHours == 0){
      Alarm_AM_PM = AM;       // midnight
    } 
    else if(AlarmHours < 12){
      Alarm_AM_PM = AM;       // 1–11 AM
    }
    else {
      Alarm_AM_PM = PM;       // 12–23
    }
  }
  else {
    // In 12-hour mode, string has AM/PM
    if(newAlarmString[strlen(newAlarmString) - 2] == 'A'){
      Alarm_AM_PM = AM;
    }
    else {
      Alarm_AM_PM = PM;
    }
  }

  // legalize values
  if(is24hourMode){
    if(AlarmHours > 23) {
      AlarmHours %= 24;
    }
  } 
  else {
    if(AlarmHours > 12) {
      AlarmHours %= 12;
    }
    if(AlarmHours == 0) {
      AlarmHours = 12;
    }
  }
  if(AlarmMinutes > 59) {
    AlarmMinutes %= 60;
  }
}   


/* raise AlarmPlaying flag */
void Alarm_Enable(void) {
  AlarmPlaying = TRUE;
}


/* lower AlarmPlaying flag */
void Alarm_Disable(void){
  AlarmPlaying  = FALSE;
}


/* check and start playing alarm if time is met */
void Alarm_Check(void){
  if(!AlarmPlaying){
    if(OnWebsite && (TimeHours == 0) && (TimeMinutes == 0)) {
      OnWebsite = !OnWebsite;
      return;
    }
    else if( (TimeHours == AlarmHours) && (TimeMinutes == AlarmMinutes) && (AM_or_PM == Alarm_AM_PM))
    {
      Alarm_Enable();
      Speaker_On();   // start alarm sound
    }
    else if (is24hourMode && (TimeHours == AlarmHours) && (TimeMinutes == AlarmMinutes))
    {
      Alarm_Enable();
      Speaker_On();   // start alarm sound
    }
  }
}


/* stop playing alarm */
void Alarm_Stop(void){
  Speaker_Off();
  if(AlarmPlaying){
    Alarm_Disable();
    Alarm_Reset();
  }
}


/* reset alarm to none */
void Alarm_Reset(void) {
  AlarmHours = -1;
  AlarmMinutes = -1;
  Alarm_AM_PM = AM;
  isAlarmSet = FALSE;
}


/* get current alarm as string in format "hh:mm AM" or "hh:mm PM" */
char* getAlarmTimeString(void) {
  static char buf[ALARM_STRING_SIZE];
  if(is24hourMode) {
    sprintf(buf, "%02u:%02u %s", AlarmHours, AlarmMinutes, "  ");
  }
  else{
    sprintf(buf, "%02u:%02u %s", AlarmHours, AlarmMinutes, (Alarm_AM_PM == AM ? "AM" : "PM"));
  }
  return buf;
}