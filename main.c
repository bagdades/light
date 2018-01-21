/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  14.12.17 19:37:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  vovan (), volodumurkoval0@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "light.h"
#include "usart.h"
#include "parser.h"
#include "scheduler.h"
#include <util/delay.h>

const char text[] PROGMEM = "Flash text proba.";
const char error[] PROGMEM = "error\r\n";
const char ok[] PROGMEM = "ok\r\n";
const char largeValue[] PROGMEM = "large value\r\n";
const char start[] PROGMEM = "start\r\n";
const char tempOut[] PROGMEM = "Temperature = ";
char number[6] = {' ', ' ', ' ', ' ', ' ', '\0'};

void TemperatureToUsart(void)
{
	UsartSendString(number);
	UsartPutChar('\n');
}


void ParserHandler(uint8_t argc, char *argv[])
{
	const char *resp = error;
	uint8_t value = 0;
	if (ParserEqualString(argv[0], "Test")) {
		if (argc > 1) {
			value = ParserStringToUchar(argv[1]);
			if (value < 100) {
				resp = ok;
			} else {
				resp = largeValue;
			}
		}	
	}
	if(ParserEqualString(argv[0], "Temp"))
	{
		resp = tempOut;
		SchedulerAddTask(TemperatureToUsart, 0, 0);
	}
	UsartSendStringFlash(resp);
}

void ListenUsart(void)
{
	uint8_t symbol;
	if (UsartGetRxCount()) {
		symbol = UsartGetChar();
		ParserParse(symbol);
	}
}

void MeasureTemp(void)
{
	int16_t temperature;
	temperature = ADCRead(0);
	temperature = ConvertADCTemp(temperature);
	IntToString(temperature, number);
	/* UsartSendString(number); */
	/* UsartPutChar('\n'); */
}

int main(void) {
	UsartInit(9600);
	SchedulerInit();
	ADCInit();
	sei();
	UsartSendString("Hello, World!\n");
	UsartSendStringFlash(text);
	SchedulerAddTask(ListenUsart, 1, 10);
	SchedulerAddTask(MeasureTemp, 100, 1000);
	while(1) {
		SchedulerDispatch();
	}
}
