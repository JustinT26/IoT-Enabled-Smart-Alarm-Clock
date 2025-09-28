/* ADC_Lab3.h
   Justin Thai
   Walter Contreras
   09/20/2025
*/

#ifndef ADC_LAB3_H
#define ADC_LAB3_H


#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer4A.h"
#include <stdint.h>


/* ADC7 on PD3 */
#define     PORT_D_MASK		0x08
#define     PIN_3_MASK		0x08


/* size of buffer to store ADC samples */
#define     BUFSIZE         1000    


/* IR sensor calibration constants */
#define     A               145120
#define     B               -214
#define     IRmax           900
#define     Dmax            400


/* max distance detected in order to turn off alarm */
#define     DISTANCE_THRESHOLD_CM   25


/* ADC sampling variables */
extern uint32_t DataBuf[BUFSIZE];   // buffer to store samples of ADC
extern volatile uint32_t Num;       // index into buffer
extern volatile uint32_t Signal;    // average value of samples in buffer
extern volatile uint32_t Distance;  // calculated distance based on the signal (average analog value)


/* initialize ADC on PD3 */
void ADC_Init(void);    


/* busy-wait analog to digital conversion */
int32_t ADC_In(void);


/* samples ADC at 1000 Hz, stores sample into buffer */
void RealTimeTask(void);


/* convert digital reading to distance in mm */
int32_t IR_Convert(int32_t n);


/* calculate the average value of the analog samples in DataBuf */
uint32_t CalculateSignal(void);


void Timer1_Init(void);

#endif