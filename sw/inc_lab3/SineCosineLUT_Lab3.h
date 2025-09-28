/* SineCosineLUT_Lab3.h
   Justin Thai
   Walter Contreras
   09/20/2025
*/

#include "stdint.h"


#ifndef SINE_COSINE_LUT_LAB3_H
#define SINE_COSINE_LUT_LAB3_H


/* scale factor to avoid using FP */
#define FP_SCALE 1024


/* cos value for theta increments of 6 (i.e. 0, 6, 12, ... 60) multiplied by a factor of 1024 */
extern const int16_t cosTable60[60];


/* sin value for theta increments of 6 (i.e. 0, 6, 12, ... 60) multiplied by a factor of 1024 */
extern const int16_t sinTable60[60];

#endif