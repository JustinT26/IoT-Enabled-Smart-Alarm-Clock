/* FSM_Lab3.c
   Justin Thai
   Walter Contreras
   09/16/2025
*/

#include "FSM_Lab3.h"


/* FSM for navigating through menus */
const State_t FSM[6] = {
    //state 0: mainMenu_setAlarmOption
    {{mainMenu_setAlarmOption, mainMenu_setTimeOption, alarmMenu_cancelAlarmOption, mainMenu_setAlarmOption}},

    //state 1: mainMenu_setTimeOption
    {{mainMenu_setTimeOption, mainMenu_setAlarmOption, timeMenu_increaseOption, mainMenu_setTimeOption}},

    //state 2: alarmMenu_cancelAlarmOption
    {{alarmMenu_cancelAlarmOption, alarmMenu_setAlarmOption, mainMenu_setAlarmOption, alarmMenu_cancelAlarmOption}},

    //state 3: alarmMenu_setAlarmOption
    {{alarmMenu_setAlarmOption, alarmMenu_cancelAlarmOption, timeMenu_increaseOption, alarmMenu_setAlarmOption}},

    //state 4: timeMenu_increaseOption
    {{timeMenu_increaseOption, timeMenu_okOption, timeMenu_increaseOption, timeMenu_increaseOption}},

    //state 5: timeMenu_okOption
    {{timeMenu_okOption, timeMenu_increaseOption, timeMenu_okOption, timeMenu_okOption}}
};