/* AnalogClock_Lab3.h
   Justin Thai
   Walter Contreras
   09/20/2025
*/

#ifndef ANALOG_CLOCK_LAB3_H
#define ANALOG_CLOCK_LAB3_H

/* array for clock .bmp image*/
extern const unsigned short clock_bmp[];

#define     CLOCK_BMP_X         25
#define     CLOCK_BMP_Y         90
#define     CLOCK_BMP_WIDTH     80
#define     CLOCK_BMP_HEIGHT    80

/* starting (x1, y1) for clock hands (hour, minute, and seconds)*/
#define     CLOCK_HAND_X1        64
#define     CLOCK_HAND_Y1        51

/* hour hand constants */
#define     HOUR_HAND_RADIUS     14
#define     HOUR_HAND_COLOR      0xFFE0 // cyan

/* minute hand constants */
#define     MINUTE_HAND_RADIUS   21
#define     MINUTE_HAND_COLOR    0xFFE0 // cyan

/* seconds hand constants */
#define     SECONDS_HAND_RADIUS  23
#define     SECONDS_HAND_COLOR   0x001F // red

#endif