/*
 * =====================================================================================
 *
 *       Filename:  light.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  14.12.17 22:36:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  vovan (), volodumurkoval0@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "light.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>


const float c1 = 24;
const float v1 = 252;
const float c2 = 230;
const float v2 = 159;

void InitSystem(void)
{

}

void Timer0Init(void)
{
#if(F_CPU != 9216000)
#error ***You must set TCCR0
#endif
	TCCR0 |= (1 << CS02) | (1 << CS00);  //T0_PRESC = 1024
	TCNT0 = T0_INIT;
	TIMSK |= (1 << TOIE0);  //enable interrupt overllow timer0
}

void ADCInit(void)
{
#if(F_CPU != 9216000)
#error ***You must set TCCR0
#endif
	ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADEN); //PRESC F_CPU/64
	ADMUX|=ADC_VREF_TYPE; //intern. 2,56V
}

int16_t ADCRead(uint8_t chanel)
{
	ADMUX = ADC_VREF_TYPE | chanel;
		_delay_us(10);//delay needed for stabilization of the ACD input voltag
	ADCSRA |= (1 << ADSC);//start convertion
	while ((ADCSRA & (1 << ADIF)) == 0)
		;//wait for complete conversion
	ADCSRA |=(1 << ADIF);
		/* flag.adc_read = FALSE; */
		return ADCW;
}

 /* Датчик має майже лінійну характеристику.
 * Формула лінійної функції f(x)=k*x+b
 * k=(y2-y1)/(x2-x1)
 * b=f(x)-((y2-y1)/(x2-x1))*x
 * звідси
 * f(x)=((y2-y1)/(x2-x1))*(x-x1)+y1
 * f(x)=((y2-y1)/(x2-x1))*x-((y2-y1)/(x2-x1))*x1+y1
 */
int ConvertTempADC(int value){
	float n =((v1-v2)/(c1-c2));
	return ((int)((n * (float)value) - (n *c1) + v1));
}
int ConvertADCTemp(int value){
	float n =((c1-c2)/(v1-v2));
	return ((int)((n * (float)value) - (n * v1) + c1));
}

ISR(TIMER0_OVF_vect)
{

}
