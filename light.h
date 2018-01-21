#ifndef  LIGHT_INC
#define  LIGHT_INC
#include <inttypes.h>

/* # define F_CPU		9216000UL  */

/* //TIMER */
/* #define T0_PRESC	1024UL */
/* #define T0_FREQ		1000UL */
/* #define T0_INIT		255 - (F_CPU/(T0_FREQ * T0_PRESC)) */

#define ADC_VREF_TYPE ((1<<REFS1)|(1<<REFS0))

//Prototypes
void InitSystem(void);
/* void Timer0Init(void); */
void ADCInit(void);
int16_t ADCRead(uint8_t chanel);
int ConvertTempADC(int value);
int ConvertADCTemp(int value);
void IntToString(int16_t value, char *strBuf);
#endif   /* ----- #ifndef LIGHT_INC  ----- */
