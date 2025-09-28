/* ManageTime_Lab3.h
   Justin Thai
   Walter Contreras
   09/18/2025
*/

#ifndef MANAGE_TIME_LAB3_H
#define MANAGE_TIME_LAB3_H


#include <stdint.h>
#include <stdio.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer3A.h"
#include "UpdateDisplay_Lab3.h"


/* length for string representation of time */
#define  TIME_STRING_SIZE  16


// set to 1 each time minutes rolls over; clear it in main
extern volatile uint8_t MinuteTick;


// initialize time module
void ManageTime_Init(uint8_t hr, uint8_t min, uint8_t sec, uint32_t priority);


// set the time 
void Time_Set(uint8_t hr, uint8_t min, uint8_t sec);


/* called every second to update time variables, draw analog and digital clock on main menu, and check for alarm */
void Time_ISR_1Hz(void);


/* get the string representation of current time */
char* getTimeString(void);


/* update the time frrom a string represenation */
void Update_Time(char* newTimeString);


/* decrement hours */
void DecrementHours(void);

/* decrement minutes */
void DecrementMinutes(void);

/*v decrement seconds*/
// void DecrementSeconds(void);

/* increment hours */
void IncrementHours(void);

/* increment minutes */
void IncrementMinutes(void);

void UpdateAM_or_PM(void);

/* increment seconds */
// void IncrementSeconds(void);


#endif