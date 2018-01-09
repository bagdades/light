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

const char text[] PROGMEM = "Flash text proba.";

int main(void) {
	UsartInit(19200);
	sei();
	UsartSendString("Hello, World!");
	UsartSendStringFlash(text);
	while(1);
}
