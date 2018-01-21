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


static const float c1 = 24;    /* Lower temperature */
static const float v1 = 277;   /* ADC value for lower temperature */
static const float c2 = 215;   /* Hight temperature */
static const float v2 = 154;   /* ADC value for hight temperature */

void InitSystem(void)
{

}

void IntToString(int16_t value, char *strBuf)
{
	uint8_t i;
	int16_t temp = value;
	uint8_t zeroFlag = 0;
	uint16_t div = 10000;

	for (i = 0; i < 4; ++i) {
		strBuf[i] = ' ';
	}
	for (i = 0; i < 4; ++i) {
		temp = temp / div;
		if(temp != 0 || zeroFlag)
		{
			strBuf[i] = temp + '0';
			zeroFlag = 1;
		}
		temp = value % div;
		div /= 10;
	}
	strBuf[i] = temp + '0';
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



