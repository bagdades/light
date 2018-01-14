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
#include <util/delay.h>

const char text[] PROGMEM = "Flash text proba.";
const char error[] PROGMEM = "error\r";
const char ok[] PROGMEM = "ok\r";
const char largeValue[] PROGMEM = "large value\r";
const char strt[] PROGMEM = "start\r";

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

int main(void) {
	uint8_t symbol;
	UsartInit(9600);
	sei();
	UsartSendString("Hello, World!");
	UsartSendStringFlash(text);
	while(1) {
		_delay_ms(20);
		if (UsartGetRxCount()) {
			symbol = UsartGetChar();
			ParserParse(symbol);
		}
	}
}
