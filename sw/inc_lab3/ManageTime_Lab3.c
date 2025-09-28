/* ManageTime_Lab3.c
   Justin Thai
   Walter Contreras
   09/18/2025
*/

#include "ManageTime_Lab3.h"
#include "Alarm_Lab3.h"


#define TICKS_1S 80000000  // 80 MHz


volatile uint8_t MinuteTick  = 0;


// initialize time module
void ManageTime_Init(uint8_t hr, uint8_t min, uint8_t am_or_pm, uint32_t priority){
  Time_Set(hr, min, 45); // last param is for seconds on initial startup 
  MinuteTick = 0;
  AM_or_PM = am_or_pm;
  Timer3A_Init(Time_ISR_1Hz, TICKS_1S, priority);  // 1 Hz
}


// set the time 
void Time_Set(uint8_t hr, uint8_t min, uint8_t sec){
  if(is24hourMode){
    if(hr > 23){
      hr %= 24;
    }
  } 
  else {
    if(hr > 12){
      hr %= 12;
    }
    if(hr == 0){
      hr = 12; // prevent 0 in 12-hour representation
    } 
  }
  
  if(min > 59){
    min %= 60;
  }
  if(sec > 59){
    sec %= 60;
  }

  TimeHours   = hr;
  TimeMinutes = min;
  TimeSeconds = sec;
  MinuteTick  = 0;
}


/* called every second to update time variables, draw analog and digital clock on main menu, and check for alarm */
void Time_ISR_1Hz(void){
  TimeSeconds++;
  inactivityCounter++; // increment every second
  DrawAnalogClockHands(); // draw clock hands

  if(TimeSeconds > 59){
    TimeSeconds = 0;
    TimeMinutes++;
    MinuteTick = 1;

    if(TimeMinutes > 59){
      TimeMinutes = 0;
      // hour rollover
      if(is24hourMode){
        TimeHours++;
        if(TimeHours > 23){
          TimeHours = 0;
        }
      } else {
        TimeHours++;
        if(TimeHours > 12){
          TimeHours = 1;
        }
        // toggle AM/PM when passing 12:00
        if(TimeHours == 12){
          AM_or_PM = !AM_or_PM;
        }
      }
    }
  }

  if(!isGuyDancing){
    MainMenu_UpdateTime(getTimeString()); //update time on main menu
  }
  Alarm_Check(); // check if alarm should go off
}



/* get the string representation of current time */
char* getTimeString(void){
  static char buf[TIME_STRING_SIZE];
  if(is24hourMode) {
    sprintf(buf, "%02u:%02u %s", TimeHours, TimeMinutes, "  ");
  }
  else{
    sprintf(buf, "%02u:%02u %s", TimeHours, TimeMinutes, (AM_or_PM == AM ? "AM" : "PM"));
  }
  return buf; 
}


/* update the time frrom a string represenation */
void Update_Time(char* newTimeString){
  TimeHours = ((newTimeString[0] - '0') * 10) + (newTimeString[1] - '0'); 
  TimeMinutes = ((newTimeString[3] - '0') * 10) + (newTimeString[4] - '0');
  TimeSeconds = 0; 
}


/* decrement hours */
void DecrementHours(void){
  if(is24hourMode){
    if(TimeHours > 0){
      TimeHours--;
    }
  }
  else{ //12 hour mode
    if(TimeHours > 1) {
      TimeHours--;
    }
  }
}

void DecrementMinutes(void){
  if(TimeMinutes > 0) {
      TimeMinutes--;
  }
}

// void DecrementSeconds(void){
//   if(TimeSeconds > 0){
//     TimeMinutes --;
//   }
// }


/* increment hours */
void IncrementHours(void){
  if(is24hourMode){
    if(TimeHours < 23) {
      TimeHours++;
    }
  }
  else{ //12 hour mode
    if(TimeHours < 12) {
      TimeHours++;
    }
  }
}

void IncrementMinutes(void){
  if(TimeMinutes < 59) {
      TimeMinutes++;
  }
}

void UpdateAM_or_PM(void){
  AM_or_PM = !AM_or_PM;
}

// void IncrementSeconds(void){
//   if(TimeMinutes < 60){
//     TimeSeconds++; 
//   }
// }
