#ifndef  PARSER_INC
#define  PARSER_INC

#include <avr/io.h>
#include <avr/pgmspace.h>

#define SIZE_RECEIVE_BUFFER		16
#define AMOUNT_PAIR				2


enum {
		FALSE = 0,
		TRUE
	};

//Prototypes
extern void ParserHandler(uint8_t argc, char *argv[]);

void ParserInit(void);
void ParserParse(char chr);
uint8_t ParserEqualStringFlash(char *str1, const char *str2);
uint8_t ParserEqualString(char *str1, char *str2);
uint8_t ParserStringToUchar(char *str);
uint16_t ParserStringToUint(char *str);
#endif   /* ----- #ifndef PARSER_INC  ----- */
