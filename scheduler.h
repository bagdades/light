#ifndef  SCHEDULER_INC
#define  SCHEDULER_INC

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "parser.h"

#define MAXTASKS	8

//TIMER
#define T0_PRESC	1024UL
#define T0_FREQ		1000UL
#define T0_INIT		255 - (F_CPU/(T0_FREQ * T0_PRESC))

typedef struct _task {
	void (*pfunc)(void);
	uint16_t delay;
	uint16_t period;
	uint8_t run;
}task;

//Protorypes
void SchedulerInit(void);
uint8_t SchedulerAddTask(void (*taskFunc)(void), uint16_t taskDelay, uint16_t taskPeriod);
void SchedulerDispatch(void);
void SchedulerDeleteTask(void (*taskFunc)(void));
#endif   /* ----- #ifndef SCHEDULER_INC  ----- */
