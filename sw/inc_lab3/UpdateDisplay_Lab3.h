/* UpdateDisplay_Lab3.h
   Justin Thai
   Walter Contreras
   09/20/2025
*/

#ifndef UPDATE_DISPLAY_LAB3_H
#define UPDATE_DISPLAY_LAB3_H


#include "../inc/ST7735.h"
#include "Globals_Lab3.h"


/* max seconds user can be AFK before forcing return to main menu */
#define  INACTIVITY_MAX_SEC   10 


/* flag to see if darkmode is enabled */
extern volatile uint8_t darkmode;


/* draw menu or move cursor, based on current state */
void UpdateDisplay(uint8_t currentState);


/* draw main menu options, digital time, and analog clock with hands */
void DrawMainMenu(void);


/* visually move the cursor on the main menu */
void MainMenu_MoveCursor(void);


/* update the time displayed on the main menu from string representation of new time */
void MainMenu_UpdateTime(char* timeString);


/* draw alarm menu options and current alarm set */
void DrawAlarmMenu(void);


/* visually move the cursor on the alarm menu */
void AlarmMenu_MoveCursor(void);


/* draw time menu options with the current time as the initial starting place */
void DrawTimeMenu(void);


/* visually move the cursor on the time menu */
void TimeMenu_MoveCursor(void);


/* increment the digit in the current position in the set time/alarm menu */
void Alarm_Or_Time_IncrementDigit(void);


/* toggle AM or PM display in the set time/alarm menu */
void TimeMenu_ToggleAM_PM(void);


/* visually move the cursor left in the set time/alarm menu */
void TimeMenu_MoveArrowLeft(void);


/* draw hour, min, and seconds hands on analog clock in main manu */
void DrawAnalogClockHands(void);


/* draw hand on analog clock in main menu */
void drawHand(uint8_t idx6deg, uint8_t radiusPixels, uint16_t color);


/* toggles darkmode on LCD */
void toggleDarkMode(void);


/* draws image of guy dancing on LCD */
void DrawDanceMeme(void);

#endif