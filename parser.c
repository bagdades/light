/*
 * =====================================================================================
 *
 *       Filename:  parser.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  14.01.18 16:32:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  vovan (), volodumurkoval0@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "parser.h"

char parserBuffer[SIZE_RECEIVE_BUFFER];
char *argv[AMOUT_PAIR];
uint8_t argc;

static uint8_t parserBufferCount = 0;
uint8_t wordFlag = 0;

void ParserInit(void)
{
	argc = 0;
	argv[0] = parserBuffer;
	wordFlag = FALSE;
	parserBufferCount = 0;
}

void ParserParse(char chr)
{
	if (chr != '\r') {
		if (parserBufferCount < SIZE_RECEIVE_BUFFER - 1) {
			if (chr != ' ') {
				if (!argc) {
					argv[0] = parserBuffer;
					argc++;
				}
				if (wordFlag) {
					if (argc < AMOUT_PAIR) {
						argv[argc] = &parserBuffer[parserBufferCount];
						argc++;
					}
					wordFlag = FALSE;
				}
				parserBuffer[parserBufferCount] = chr;
				parserBufferCount++;
			} else {
				if (!wordFlag) {
					parserBuffer[parserBufferCount] = 0;
					parserBufferCount++;
					wordFlag = TRUE;
				}
			}
		}
		parserBuffer[parserBufferCount] = 0;
		return;
	} else {
		parserBuffer[parserBufferCount] = 0;
		if (argc) {
			ParserHandler(argc, argv);
		}
		argc = 0;
		wordFlag = FALSE;
		parserBufferCount = 0;
	}
}

uint8_t ParserEqualStringFlash(char *str1, const char *str2)
{
	uint8_t i = 0;
	while(str1[i] == pgm_read_byte(&str2[i]) && str1[i] != '\0' && pgm_read_byte(&str2[i]) != '\0') {
		i++;
	}
	if (str1[i] == '\0' && pgm_read_byte(&str2[i]) == '\0') {
		return TRUE;
	} else {
		return FALSE;
	}
}

uint8_t ParserEqualString(char *str1, char *str2)
{
	uint8_t i = 0;
	while(str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0') {
		i++;
	}
	if (str1[i] == '\0' && str2[i] == '\0') {
		return TRUE;
	} else {
		return FALSE;
	}
}

uint8_t ParserStringToUchar(char *str)
{
	uint8_t value = 0;
	while(*str == '0') str++;
	while(*str) {
		value += (*str - 0x30);
		str++;
		if (*str) {
			value *= 10;
		}
	}
	return value;
}

uint16_t ParserStringToUint(char *str)
{
	uint16_t value = 0;
	while(*str == '0') str++;
	while(*str) {
		value += (*str - 0x30);
		str++;
		if (*str) {
			value *= 10;
		}
	}
	return value;
}
