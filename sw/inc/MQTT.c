/* MQTT.c
   Justin Thai
   Walter Contreras
   09/24/2025
*/


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "PLL.h"

#include "UART.h"
#include "UART5.h"
#include "esp8266.h"

#include "../inc_lab3/UpdateDisplay_Lab3.h"
#include "../inc_lab3/Globals_Lab3.h"
#include "../inc_lab3/ManageTime_Lab3.h"
#include "../inc_lab3/FSM_Lab3.h"
#include "../inc_lab3/Alarm_Lab3.h"
#include "../inc_lab3/SpeakerInterface_Lab3.h"


#define DEBUG1                // First level of Debug
//#undef  DEBUG1                // Comment out to enable Debug1

#define UART5_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART5_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART5_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART5_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART5_CTL_UARTEN         0x00000001  // UART Enable


// ----   Function Prototypes not defined in header files  ------
void EnableInterrupts(void);    // Defined in startup.s

//Buffers for send / recv
char                    input_char;
char                    b2w_buf[64];
char                    w2b_buf[128];
static uint32_t         bufpos          = 0;



// --------------------------     W2B Parser      ------------------------------
//
// This parser decodes and executes commands from the MQTT Web Appication 
//
void Parser(void) {
  //Perform operations on the w2b buffer to set variables as appropriate
	//atoi() and strtok() may be useful
	
  //NEED TO HAVE COMMAS IN THE MESSAGE YOU SEND
	char first_token[128];
	strcpy(first_token, strtok(w2b_buf, ","));
  OnWebsite = TRUE;
	
  if(strcmp(first_token, "12/24") == 0) { // handle toggle 24 hour mode
    toggle24HourMode();
  }
  else if(strcmp(first_token, "toggle darkmode") == 0) { //handle toggle dark mode
    toggleDarkMode();
  }
  else if(strcmp(first_token, "dance") == 0) { //handle dance
    isGuyDancing = TRUE;
    DrawDanceMeme();
  }
  else if(strcmp(first_token, "hour--") == 0) { //handle hour--
    DecrementHours();
    MainMenu_UpdateTime(getTimeString());
  }
  else if(strcmp(first_token, "hour++") == 0){ //handle hour++
    IncrementHours();
    MainMenu_UpdateTime(getTimeString());
  }
  else if(strcmp(first_token, "minute--") == 0){ //handle minute--
    DecrementMinutes();
    MainMenu_UpdateTime(getTimeString());
  }
  else if(strcmp(first_token, "minute++") == 0){ //handle minute++
    IncrementMinutes();
    MainMenu_UpdateTime(getTimeString());
  }
  else if(strcmp(first_token, "am or pm") == 0){ //handle toggle AM/PM
    UpdateAM_or_PM();
    MainMenu_UpdateTime(getTimeString());
  }
  else if(strcmp(first_token, "silence") == 0){ //handle silence alarm
    Alarm_Stop();
  }
  else if(strcmp(first_token, "play sound") == 0){ //handle play sound
    Speaker_On();
  }


	#ifdef DEBUG1
		UART_OutString("Token 1: ");
		UART_OutString(first_token);
		UART_OutString("\r\n");
	#endif
}
  

// -----------------------  TM4C_to_MQTT Web App -----------------------------
// This routine publishes clock data to the
// MQTT Web Application via the MQTT Broker
// The data is sent using CSV format:
//
// ----------------------------------------------------------------------------
void TM4C_to_MQTT(void){
  char msg[32];
  uint8_t mode = is24hourMode ? 1 : 0;
  snprintf(msg, sizeof(msg), "%u,%u,%u,%u,%u\n", mode, TimeHours, TimeMinutes, TimeSeconds, AM_or_PM);
  UART5_OutString(msg);
}
 

// -------------------------   MQTT_to_TM4C  -----------------------------------
// This routine receives the command data from the MQTT Web App and parses the
// data and feeds the commands to the TM4C.
// -----------------------------------------------------------------------------
void MQTT_to_TM4C(void) {

  while((UART5_FR_R & UART5_FR_RXFE) == 0) {
    char c = (char)(UART5_DR_R & 0xFF);
    if (c == '\r') continue;

    if (c == '\n') {                    // end-of-line
      w2b_buf[bufpos] = '\0';
      if (bufpos > 1 && w2b_buf[0] == '@') {
        memmove(w2b_buf, w2b_buf+1, bufpos);  // strip '@'
        Parser();                              // only clean payloads arrive here
      }
      bufpos = 0;
      return;
    }

    if (bufpos < sizeof(w2b_buf)-1) {
      w2b_buf[bufpos++] = c;
    }
    else {
      bufpos = 0;
    }
    UART_OutChar(c);         // Debug only
  }
}   