/*
 * =====================================================================================
 *
 *       Filename:  usart.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  14.12.17 22:35:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  vovan (), volodumurkoval0@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "usart.h"
#include <inttypes.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint8_t txBufferTail;
static volatile uint8_t txBufferHead;
static volatile uint8_t txBufferCount;
static volatile uint8_t rxBufferTail;
static volatile uint8_t rxBufferHead;
static volatile uint8_t rxBufferCount;

static volatile uint8_t usartTxBuffer[USART_TX_BUFFER_SIZE];
static volatile uint8_t usartRxBuffer[USART_TX_BUFFER_SIZE];

#ifndef  F_CPU
#error	"F_CPU is not defined"			/*  */
#endif     /* -----  not F_CPU  ----- */

void UsartInit(uint16_t baudRate){
	uint16_t ubrrValue;
	cli();
	ubrrValue = F_CPU/(16UL * baudRate) - 1;
	UBRRL = ubrrValue;
	UBRRH = ubrrValue >> 8;
	UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);//8-bit ,1-stop bit,no parity
	UCSRB |= (1 << TXEN) | (1 << RXEN) | (1 << TXCIE) |
			(1 << RXCIE);//enable tx ,enable interrupt tx
}

void UsartFlushTxBuffer(void)
{
	txBufferTail = 0;
	txBufferHead = 0;
	txBufferCount = 0;
}

void UsartPutChar(char ch){
	while(txBufferCount == USART_TX_BUFFER_SIZE);
	/* cli(); */
	if(bit_is_set(UCSRA,UDRE) && txBufferCount == 0)
		UDR = ch;
	else if(txBufferCount < USART_TX_BUFFER_SIZE){
		txBufferCount ++;
		usartTxBuffer[txBufferHead] = ch;
		txBufferHead ++;
		if(txBufferHead == USART_TX_BUFFER_SIZE)
			txBufferHead = 0;
	}
	/* sei(); */
}

void UsartSendString(char *string){
	while(*string){
		UsartPutChar(*string);
		string++;
	}
}

void UsartSendStringFlash(const char *string)
{
	char sym = pgm_read_byte(string);
	while(sym)
	{
		UsartPutChar(sym);
		string++;
		sym = pgm_read_byte(string);
	}
}

ISR(USART_TXC_vect){
	if(txBufferCount){
		UDR	= usartTxBuffer[txBufferTail];
		txBufferCount --;
		txBufferTail ++;
		if(txBufferTail == USART_TX_BUFFER_SIZE)
			txBufferTail = 0;
	}
}

uint8_t UsartGetRxCount(void)
{
	return rxBufferCount;
}

uint8_t  UsartGetChar(void){
	uint8_t ch = 0;
	if(rxBufferCount){
		ch = usartRxBuffer[rxBufferTail];
		rxBufferCount --;
		rxBufferTail ++;
		if(rxBufferTail == USART_RX_BUFFER_SIZE)
			rxBufferTail = 0;
	}
	return ch;
}

ISR(USART_RXC_vect){
	if(rxBufferCount < USART_RX_BUFFER_SIZE){
		usartRxBuffer[rxBufferHead] = UDR;
		rxBufferHead ++;
		rxBufferCount ++;
		if(rxBufferHead ==  USART_TX_BUFFER_SIZE)
			rxBufferHead = 0;
	}
}

