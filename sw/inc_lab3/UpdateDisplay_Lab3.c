/* UpdateDisplay_Lab3.c
   Justin Thai
   Walter Contreras
   09/20/2025
*/


#include "UpdateDisplay_Lab3.h"
#include "FSM_Lab3.h"
#include "AnalogClock_Lab3.h"
#include "ManageTime_Lab3.h"
#include "Globals_Lab3.h"
#include "Alarm_Lab3.h"
#include "SineCosineLUT_Lab3.h"
#include <string.h>
#include "DancingGuy_Lab3.h"
 

static uint8_t mainMenuCursorY = 12; // start position for main menu cursor
static uint8_t alarmMenuCursorY = 12; //start position for the alarm menu cursor
static uint8_t timeMenuCursorY = 12; 
static uint8_t timeMenuUpArrowX = 13;


static char mainMenu_buffer[16];
static char timeMenu_buffer[16];
static char alarmMenu_buffer[16];  
static char* mainMenu_timeDisplayed = mainMenu_buffer;
static char* timeMenu_timeDisplayed = timeMenu_buffer;
static char* alarmMenu_alarmDisplayed = alarmMenu_buffer;


static MenuType previousMenu = mainMenu; 
static MenuType currentMenu = mainMenu;


volatile uint8_t darkmode = TRUE;


/* draw menu or move cursor, based on current state */
void UpdateDisplay(uint8_t currentState) {
    // figure out which menu currentState belongs to
    if(currentState == mainMenu_setAlarmOption || currentState == mainMenu_setTimeOption){
        if(currentState == mainMenu_setAlarmOption){
            if(needToSetClock == TRUE) {
                Update_Time(timeMenu_timeDisplayed);
                needToSetClock = FALSE;
            }
            else if(needToSetAlarm == TRUE) {
                Alarm_Set(timeMenu_timeDisplayed);
                needToSetAlarm = FALSE;
            }
        
        }
        currentMenu = mainMenu;
    }
    else if(currentState == alarmMenu_cancelAlarmOption || currentState == alarmMenu_setAlarmOption){
        currentMenu = alarmMenu;
    }
    else if(currentState == timeMenu_increaseOption || currentState == timeMenu_okOption){
        currentMenu = timeMenu;
    }

    // redraw menu only if different
    if(currentMenu != previousMenu){
        //reset cursor positions for all screens
        mainMenuCursorY = 12;
        alarmMenuCursorY = 12;
        timeMenuCursorY = 12;
        timeMenuUpArrowX = 13;

        if(currentMenu == mainMenu) {
            DrawMainMenu();
        }
        else if(currentMenu == alarmMenu) {
            DrawAlarmMenu();
        }
        else if(currentMenu == timeMenu)  {
            DrawTimeMenu();
        }
        previousMenu = currentMenu;
    }
    else {
        // if user was AFK, reset all cursor positions
        if(inactivityCounter >= INACTIVITY_MAX_SEC) {
            mainMenuCursorY = 12;
            alarmMenuCursorY = 12;
            timeMenuCursorY = 12;
            timeMenuUpArrowX = 13;

            if(isGuyDancing){
                DrawMainMenu();
            }
            return;
        }
        // same menu: just move cursor
        if(currentMenu == mainMenu) {
            MainMenu_MoveCursor();
        }
        else if(currentMenu == alarmMenu) {
            AlarmMenu_MoveCursor();
        }
        else if(currentMenu == timeMenu) {
            TimeMenu_MoveCursor();  
        }

    }
}


/* draw main menu options, digital time, and analog clock with hands */
void DrawMainMenu(void) {
    ST7735_SetRotation(2); 
    ST7735_FillScreen(ST7735_BLACK);

    ST7735_DrawString(3, 12, " ->", ST7735_YELLOW);

    //digital time
    strcpy(mainMenu_buffer, getTimeString());
    mainMenu_timeDisplayed = mainMenu_buffer;
    ST7735_DrawString(7, 10, mainMenu_timeDisplayed, ST7735_CYAN);
    
    //draw analog clock
    ST7735_DrawBitmap(CLOCK_BMP_X, CLOCK_BMP_Y, clock_bmp, CLOCK_BMP_WIDTH, CLOCK_BMP_HEIGHT);
    DrawAnalogClockHands();

    // menu text
    ST7735_DrawString(7, 12, "Set Alarm", ST7735_WHITE);
    ST7735_DrawString(7, 13, "Set Time", ST7735_WHITE);
}


/* visually move the cursor on the main menu */
void MainMenu_MoveCursor(void){
    // erase old arrow
    ST7735_DrawString(3, mainMenuCursorY, " ->", ST7735_BLACK);

    // move y position
    mainMenuCursorY++;
    if(mainMenuCursorY > 13){
        mainMenuCursorY = 12;
    }
    // draw new arrow
    ST7735_DrawString(3, mainMenuCursorY, " ->", ST7735_YELLOW);

}


/* update the time displayed on the main menu from string representation of new time */
void MainMenu_UpdateTime(char* timeString) {
    if(currentState == mainMenu_setAlarmOption || currentState == mainMenu_setTimeOption) {
        ST7735_DrawString(7, 10, timeString, ST7735_BLACK);  //erase old time
        ST7735_DrawString(7, 10, getTimeString(), ST7735_CYAN);  //draw old time
    }
}


/* draw alarm menu options and current alarm set */
void DrawAlarmMenu(void) {
    ST7735_SetRotation(2); 
    ST7735_FillScreen(ST7735_BLACK);

    ST7735_DrawString(4, 3, "Current Alarm", ST7735_WHITE);
    if(isAlarmSet) {
        strcpy(alarmMenu_buffer, getAlarmTimeString());
    } else {
        strcpy(alarmMenu_buffer, "--:-- --");
    }
    alarmMenu_alarmDisplayed = alarmMenu_buffer;
    ST7735_DrawString(7, 7, alarmMenu_alarmDisplayed, ST7735_ORANGE);

    ST7735_DrawString(2, 12, " ->", ST7735_YELLOW);
    // menu text
    ST7735_DrawString(6, 12, "Cancel Alarm", ST7735_WHITE);
    ST7735_DrawString(6, 13, "Change Alarm", ST7735_WHITE);
}


/* visually move the cursor on the alarm menu */
void AlarmMenu_MoveCursor(void){
    // erase old arrow
    ST7735_DrawString(2, alarmMenuCursorY, " ->", ST7735_BLACK);

    // move y position
    alarmMenuCursorY++;
    if(alarmMenuCursorY > 13){
        alarmMenuCursorY = 12;
    }

    // draw new arrow
    ST7735_DrawString(2, alarmMenuCursorY, " ->", ST7735_YELLOW);
}


/* draw time menu options with the current time as the initial starting place */
void DrawTimeMenu(void) 
{
    ST7735_SetRotation(2); 
    ST7735_FillScreen(ST7735_BLACK);

    if(needToSetClock == TRUE) {
        ST7735_DrawString(7, 1, "Set Time", ST7735_GREEN);

        strcpy(timeMenu_buffer, getTimeString());
        timeMenu_timeDisplayed = timeMenu_buffer;  

        ST7735_DrawString(7, 5, timeMenu_timeDisplayed, ST7735_CYAN);
    }
    else if(needToSetAlarm == TRUE) 
    {
        ST7735_DrawString(6, 1, "Set Alarm", ST7735_ORANGE);

        if(isAlarmSet) {
            strcpy(timeMenu_buffer, getAlarmTimeString());
        } else {
            strcpy(timeMenu_buffer, getTimeString());
        }
        timeMenu_timeDisplayed = timeMenu_buffer;

        if(isAlarmSet) {
            ST7735_DrawString(7, 5, timeMenu_timeDisplayed, ST7735_ORANGE);
        }
        else {
            ST7735_DrawString(7, 5, timeMenu_timeDisplayed, ST7735_CYAN);
        }
    }

    if(is24hourMode){ // dont allow user to toggle AM/PM, so force cursor to move left to next position
        pos = 1;
        timeMenuUpArrowX-=2;
    }
    ST7735_DrawString(timeMenuUpArrowX, 6, "^", ST7735_YELLOW);
    ST7735_DrawString(2, 12, " ->", ST7735_YELLOW);
    ST7735_DrawString(6, 12, "Toggle/Increase", ST7735_WHITE);
    ST7735_DrawString(6, 13, "OK", ST7735_WHITE);
}


/* visually move the cursor on the time menu */
void TimeMenu_MoveCursor(void){
    // erase old arrow
    ST7735_DrawString(2, timeMenuCursorY, " ->", ST7735_BLACK);

    // move y position
    timeMenuCursorY++;
    if(timeMenuCursorY > 13){
        timeMenuCursorY = 12;
    }

    // draw new arrow
    ST7735_DrawString(2, timeMenuCursorY, " ->", ST7735_YELLOW);
}


/* increment the digit in the current position in the set time/alarm menu */
void Alarm_Or_Time_IncrementDigit(void) {
    uint8_t timeMenu_timeDisplayed_Len = strlen(timeMenu_timeDisplayed);
    ST7735_DrawString(7, 5, timeMenu_timeDisplayed, ST7735_BLACK);

    if(!is24hourMode) {
        if(pos == 1){
            if(timeMenu_timeDisplayed[timeMenu_timeDisplayed_Len - 4] < '9'){ 
                timeMenu_timeDisplayed[timeMenu_timeDisplayed_Len - 4]++;
            }
            else {
                timeMenu_timeDisplayed[timeMenu_timeDisplayed_Len - 4] = '0'; // wrap around
            }
        }
        else if(pos == 2){
            if(timeMenu_timeDisplayed[timeMenu_timeDisplayed_Len - 5] < '5'){ 
                timeMenu_timeDisplayed[timeMenu_timeDisplayed_Len - 5]++;
            }
            else {
                timeMenu_timeDisplayed[timeMenu_timeDisplayed_Len - 5] = '0'; // wrap around
            }
        }
        else if(pos == 3){
            if(timeMenu_timeDisplayed[timeMenu_timeDisplayed_Len - 7] < '9')
            { 
                timeMenu_timeDisplayed[timeMenu_timeDisplayed_Len - 7]++;
                if(timeMenu_timeDisplayed[0] > '0') {
                    timeMenu_timeDisplayed[0] = '0';
                }
            }
            else {
                timeMenu_timeDisplayed[timeMenu_timeDisplayed_Len - 7] = '1'; // wrap around
            }
        }
        else if(pos == 4){
            if(timeMenu_timeDisplayed[timeMenu_timeDisplayed_Len - 8] < '1'){ 
                timeMenu_timeDisplayed[timeMenu_timeDisplayed_Len - 8]++;
                if(timeMenu_timeDisplayed[1] > '2') {
                    timeMenu_timeDisplayed[1] = '1';
                }
            }
            else { 
                timeMenu_timeDisplayed[timeMenu_timeDisplayed_Len - 8] = '0'; // wrap around
            }
        }
    }
    else{ //24 hour mode logic
        if(pos == 1){ // minute ones
            timeMenu_timeDisplayed[4]++;
            if(timeMenu_timeDisplayed[4] > '9') {
                timeMenu_timeDisplayed[4] = '0';
            }
        }
        else if(pos == 2){ // minute tens
            timeMenu_timeDisplayed[3]++;
            if(timeMenu_timeDisplayed[3] > '5'){
                timeMenu_timeDisplayed[3] = '0';
            }
        }
        else if(pos == 3){ // hour ones
            timeMenu_timeDisplayed[1]++;
            if(timeMenu_timeDisplayed[0] == '2'){  // max 23 hours
                if(timeMenu_timeDisplayed[1] > '3'){
                    timeMenu_timeDisplayed[1] = '0';
                }
            } 
            else {                              // max 19 hours
                if(timeMenu_timeDisplayed[1] > '9'){
                    timeMenu_timeDisplayed[1] = '0';
                }
            }
        }
        else if(pos == 4){ // hour tens
            timeMenu_timeDisplayed[0]++;
            if(timeMenu_timeDisplayed[0] > '2'){
                timeMenu_timeDisplayed[0] = '0';
            }
            // if tens is '2', ones must be 0–3
            if(timeMenu_timeDisplayed[0] == '2' && timeMenu_timeDisplayed[1] > '3'){
                timeMenu_timeDisplayed[1] = '0';
            }
        }
    }

    if(isAlarmSet && needToSetAlarm) {
        ST7735_DrawString(7, 5, timeMenu_timeDisplayed, ST7735_ORANGE);
    }
    else {
        ST7735_DrawString(7, 5, timeMenu_timeDisplayed, ST7735_CYAN);
    }
}


/* toggle AM or PM display in the set time/alarm menu */
void TimeMenu_ToggleAM_PM(void) {
    AM_or_PM = !AM_or_PM;
    ST7735_DrawString(7, 5, timeMenu_timeDisplayed, ST7735_BLACK);
    timeMenu_timeDisplayed[strlen(timeMenu_timeDisplayed) - 2] = (AM_or_PM == AM) ? 'A' : 'P'; // change last two chars to AM or PM
    if(isAlarmSet && needToSetAlarm) {
        ST7735_DrawString(7, 5, timeMenu_timeDisplayed, ST7735_ORANGE);
    }
    else {
        ST7735_DrawString(7, 5, timeMenu_timeDisplayed, ST7735_CYAN);
    }
}


/* visually move the cursor left in the set time/alarm menu */
void TimeMenu_MoveArrowLeft(void) {
    ST7735_DrawString(timeMenuUpArrowX, 6, "^", ST7735_BLACK); // erase old arrow
    if(pos == 0 || pos == 2) {
        timeMenuUpArrowX -= 2;
    }
    else{
        timeMenuUpArrowX -= 1; 
    }
    ST7735_DrawString(timeMenuUpArrowX, 6, "^", ST7735_YELLOW); // draw new arrow
}


/* draw hour, min, and seconds hands on analog clock in main manu */
void DrawAnalogClockHands(void) {
    //only draw hands if we are in main menu and guy is not dancing
    if(!isGuyDancing){
        if(currentState == mainMenu_setAlarmOption || currentState == mainMenu_setTimeOption) {
            //"erase" old clock by drawing over with blank clock bmp
            ST7735_DrawBitmap(CLOCK_BMP_X, CLOCK_BMP_Y, clock_bmp, CLOCK_BMP_WIDTH, CLOCK_BMP_HEIGHT);

            uint8_t H = TimeHours;     
            uint8_t M = TimeMinutes;   
            uint8_t S = TimeSeconds;   

            //convert to a 6 degree increment
            uint8_t idx_s = (S % 60);                              
            uint8_t idx_m = (M % 60);                               
            uint8_t idx_h = ((H % 12) * 5 + (M / 12)) % 60;       

            drawHand(idx_h, HOUR_HAND_RADIUS,    HOUR_HAND_COLOR);
            drawHand(idx_m, MINUTE_HAND_RADIUS,  MINUTE_HAND_COLOR);
            drawHand(idx_s, SECONDS_HAND_RADIUS, SECONDS_HAND_COLOR);
        }
    }
}


/* draw hand on analog clock in main menu */
void drawHand(uint8_t idx6deg, uint8_t radiusPixels, uint16_t color) {
    int center_x = CLOCK_HAND_X1;  
    int center_y = CLOCK_HAND_Y1;  

    int index = (idx6deg + 45) % 60; // + 45 to rotate 90 degrees to fix alignment issue

    int16_t c = cosTable60[index];   
    int16_t s = sinTable60[index];   
    int x2 = center_x + ((radiusPixels * c) + (FP_SCALE/2)) / FP_SCALE;
    int y2 = center_y + ((radiusPixels * s) + (FP_SCALE/2)) / FP_SCALE;

    ST7735_DrawLine(center_x, center_y, x2, y2, color);
}


/* toggles darkmode on LCD */
void toggleDarkMode(void) {
    ST7735_InvertDisplay(darkmode);
    darkmode = !darkmode;
}

/* draws image of guy dancing on LCD */
void DrawDanceMeme(void){
    ST7735_FillScreen(ST7735_BLACK);
    if(DancingGuyPosition == position1){
        ST7735_DrawBitmap(25, 140, dancingGuy1, 80, 140);
    }
    else if(DancingGuyPosition == position2){
        ST7735_DrawBitmap(25, 140, dancingGuy2, 80, 140);
    }
    DancingGuyPosition = !DancingGuyPosition;
}