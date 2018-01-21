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
const char strt[] PROGMEM = "start\r\n";

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
	UsartSendStringFlash(resp);
}

void Task1(void)
{
	UsartSendString("1\n");
}

void Task2(void)
{
	UsartSendString("2\n");
}

void Task3(void)
{
	SchedulerDeleteTask(Task2);
	UsartSendString("3\n");
}

void Task4(void)
{
	uint8_t symbol;
	if (UsartGetRxCount()) {
		symbol = UsartGetChar();
		ParserParse(symbol);
	}
}

int main(void) {
	UsartInit(9600);
	SchedulerInit();
	sei();
	UsartSendString("Hello, World!\n");
	UsartSendStringFlash(text);
	SchedulerAddTask(Task1, 0, 2000);
	SchedulerAddTask(Task2, 0, 2000);
	SchedulerAddTask(Task3, 3000, 0);
	SchedulerAddTask(Task4, 1, 10);
	while(1) {
		SchedulerDispatch();
	}
}
