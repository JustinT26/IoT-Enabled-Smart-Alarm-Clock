/* FSM_Lab3.h
   Justin Thai
   Walter Contreras
   09/16/2025
*/

#ifndef FSM_LAB3_H
#define FSM_LAB3_H


#include <stdint.h>


/* state struct */
typedef struct {
    uint8_t next[4];
} State_t;


/* menu enums */
typedef enum {
    mainMenu,
    alarmMenu,
    timeMenu
} MenuType;


/* FSM state enums */
typedef enum {
    mainMenu_setAlarmOption,        // state 0
    mainMenu_setTimeOption,         // state 1
    alarmMenu_cancelAlarmOption,    // state 2
    alarmMenu_setAlarmOption,       // state 3
    timeMenu_increaseOption,        // state 4
    timeMenu_okOption               // state 5
} States;


/* FSM for navigating through menus */
extern const State_t FSM[6];

#endif