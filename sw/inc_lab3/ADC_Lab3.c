/* ADC_Lab3.c
   Justin Thai
   Walter Contreras
   09/20/2025
*/

#include "ADC_Lab3.h"


uint32_t DataBuf[BUFSIZE]; 
volatile uint32_t Num = 0;     
volatile uint32_t Signal = 0;
volatile uint32_t Distance = 0xFFFFFFFF;


/* initialize ADC on PD3 */
void ADC_Init(void){
  uint32_t delay;
  SYSCTL_RCGCADC_R |= 0x0001;                             // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= PORT_D_MASK;                       // 2) activate clock for Port D
  delay = SYSCTL_RCGCGPIO_R;                              // 3 for stabilization
  GPIO_PORTD_DIR_R &= ~PIN_3_MASK;                        // 4) make PD3 input
  GPIO_PORTD_AFSEL_R |= PIN_3_MASK;                       // 5) enable alternate function on PD3
  GPIO_PORTD_DEN_R &= ~PIN_3_MASK;                        // 6) disable digital I/O on PD3
  GPIO_PORTD_AMSEL_R |= PIN_3_MASK;                       // 7) enable analog functionality on PD3
  ADC0_PC_R &= ~0xF;
  ADC0_PC_R |= 0x1;                                       // 8) configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;                                  // 9) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;                                // 10) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;                                 // 11) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;
  ADC0_SSMUX3_R += 4;                                     // 12) set channel
  ADC0_SSCTL3_R = 0x0006;                                 // 13) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;                                   // 14) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;                                 // 15) enable sample sequencer 3

  Timer1_Init();
  Timer4A_Init(&RealTimeTask, 80000, 1); // 1 kHz sampling
}


/* busy-wait analog to digital conversion */
int32_t ADC_In(void){
  int32_t result;
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}


/* samples ADC at 1000 Hz, stores sample into buffer */
void RealTimeTask(void){
  uint32_t ADCvalue;
  ADCvalue = ADC_In();
  if(Num < BUFSIZE){
    DataBuf[Num] = ADCvalue;
    Num++;
  }
}


/* convert digital reading to distance in mm */
int32_t IR_Convert(int32_t n){
  int32_t d;
  if((n+ B) != 0){
    d = A/(n + B);
  }
  else{
    d = 0;
  }
  return d;
}


/* calculate the average value of the analog samples in DataBuf */
uint32_t CalculateSignal(void){
  uint32_t sum = 0; 
  for(int i = 0; i< BUFSIZE; i++){
    sum += DataBuf[i];
  }
  return(sum/BUFSIZE); 
}


// -----------          Timer1_Init           ---------------------------
// Reading TIMER1_TAR_R will return the 32-bit current time in 12.5ns units. 
// The timer counts down. 
void Timer1_Init(void){
  volatile uint32_t delay;
  SYSCTL_RCGCTIMER_R    |= 0x02;                // 0) activate TIMER1a
  delay                  = SYSCTL_RCGCTIMER_R;  // allow time to finish activating
  TIMER1_CTL_R           = 0x00000000;          // 1) disable TIMER1A during setup
  TIMER1_CFG_R           = 0x00000000;          // 2) configure for 32-bit mode
  TIMER1_TAMR_R          = 0x00000002;          // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R         = 0xFFFFFFFF;          // 4) reload value
  TIMER1_TAPR_R          = 0;                   // 5) bus clock resolution
  TIMER1_CTL_R           = 0x00000001;          // 10) enable TIMER1A
}