/* DancingGuy_Lab3.h
   Justin Thai
   Walter Contreras
   09/22/2025
*/

#ifndef DANCING_GUY_LAB3_H
#define DANCING_GUY_LAB3_H


/* enum for dance position */
typedef enum {
    position1,
    position2
} dancePosition;


/* flag for which image to draw*/
extern volatile dancePosition DancingGuyPosition;


/* array for dancing guy .bmp image1 */
extern const unsigned short dancingGuy1[];


/* array for dancing guy .bmp image2 */
extern const unsigned short dancingGuy2[];

#endif