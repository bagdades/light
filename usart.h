#ifndef  USART_INC
#define  USART_INC

#include <inttypes.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

/* #define F_CPU					9216000UL */

#define USART_BAUDRATE 			19200UL
#define USART_TX_BUFFER_SIZE	20
#define USART_RX_BUFFER_SIZE	20
#define USART_BAUDRATE_DIV 		F_CPU/(16 * USART_BAUDRATE)-1


//Prototypes
void UsartInit				(uint16_t baudRate);
void UsartFlushTxBuffer		(void);
void UsartPutChar			(char ch);
void UsartSendString		(char *string);
void UsartSendStringFlash	(const char *string);
uint8_t UsartGetChar		(void);
uint8_t UsartGetRxCount		(void);

#endif   /* ----- #ifndef USART_INC  ----- */
