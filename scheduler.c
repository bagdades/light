/*
 * =====================================================================================
 *
 *       Filename:  scheduler.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  21.01.18 13:10:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  vovan (), volodumurkoval0@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "scheduler.h"

static volatile task taskArray[MAXTASKS];

void SchedulerInit(void)
{

#if  (F_CPU != 9216000UL)
#error ***You must set TCCR0 
#endif     /* -----  (F_CPU != 9216000UL)  ----- */

	TCCR0 |= (1 << CS02) | (1 << CS00);//T0_PRESC = 1024
	TCNT0 = T0_INIT;
	TIMSK|=(1<<TOIE0);//enable interrupt overllow timer0
}

uint8_t SchedulerAddTask(void (*taskFunc)(void), uint16_t taskDelay, uint16_t taskPeriod)
{
	uint8_t n = 0;
	uint8_t i;
	uint8_t retVal = FALSE;

	cli();
	for (i = 0; i < MAXTASKS; ++i) {
		if(taskArray[i].pfunc == taskFunc) {
			taskArray[i].delay = taskDelay;
			retVal = TRUE;
		}
	}
	if(!retVal) {
		while((taskArray[n].pfunc != 0 ) && (n < MAXTASKS))
			n++;
		if (n < MAXTASKS) {
			taskArray[n].pfunc = taskFunc;
			taskArray[n].delay = taskDelay;
			taskArray[n].period = taskPeriod;
			taskArray[n].run = 0;
			retVal = TRUE;
		}
	}
	sei();
	return retVal;
}

void SchedulerDeleteTask(void (*taskFunc)(void))
{
	uint8_t ii;
	for (ii = 0; ii < MAXTASKS; ii++) {
		if (taskArray[ii].pfunc) {
			if (taskArray[ii].pfunc == taskFunc) {
				taskArray[ii].pfunc = 0x0000;
				taskArray[ii].delay = 0;
				taskArray[ii].period = 0;
				taskArray[ii].run = 0;
				break;
			}
		}
	}
}

void SchedulerDispatch(void)
{
	uint8_t i;
	for (i = 0; i < MAXTASKS; ++i) {
		if (taskArray[i].run == 1) {
			(*taskArray[i].pfunc)();
			if (taskArray[i].period == 0) {     /* Delete task if period equal zero */
				taskArray[i].pfunc = 0x0000;
				taskArray[i].delay = 0;
			}
			taskArray[i].run = 0;
		}
	}
}

ISR(TIMER0_OVF_vect)
{
	uint8_t i; 
	TCNT0 = T0_INIT;
	for (i = 0; i < MAXTASKS; ++i) {
		if (taskArray[i].pfunc) {
			if (taskArray[i].delay == 0) {
				taskArray[i].run = 1;
				taskArray[i].delay = taskArray[i].period;
			} else {
				taskArray[i].delay--;
			}
		}
	}
}
