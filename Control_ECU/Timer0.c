/*
 * Timer0.c
 *
 *  Created on: Jul 15, 2022
 *      Author: Mahmoud Yasser
 */
#include "Timer0.h"
#include "common_macros.h"
#include "std_types.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile void (*g_callBackPtr)(void) = NULL_PTR;

ISR( TIMER0_OVF_vect) {
	if (g_callBackPtr != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect) {
	if (g_callBackPtr != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

void Timer0_init(const Timer0_ConfigType *Config_Ptr) {
	if (Config_Ptr->mode == Timer0_NormalMode) {
		TCNT0 = Config_Ptr->initialCount; //Set Timer initial value
		TIMSK |= (1 << TOIE0); // Enable Timer0 Overflow Interrupt
		/* Configure the timer control register
		 * 1. Non PWM mode FOC0=1
		 * 2. Normal Mode WGM01=0 & WGM00=0
		 * 3. Normal Mode COM00=0 & COM01=0
		 * 4. clock = F_CPU/1024 CS00=1 CS01=0 CS02=1
		 */
		TCCR0 = (1 << FOC0);
		TCCR0 &= 0x80;
		TCCR0 |= (Config_Ptr->prescaler & 0x07);
	}

	else if (Config_Ptr->mode == Timer0_CompareMode) {
		TCNT0 = Config_Ptr->initialCount;    // Set Timer initial value
		OCR0 = Config_Ptr->compareModeCompareValue; // Set Compare Value
		TIMSK |= (1 << OCIE0); // Enable Timer0 Compare Interrupt
		/* Configure timer0 control register
		 * 1. Non PWM mode FOC0=1
		 * 2. CTC Mode WGM01=1 & WGM00=0
		 * 3. No need for OC0 in this example so COM00=0 & COM01=0
		 * 4. clock = F_CPU/1024 CS00=1 CS01=0 CS02=1
		 */
		TCCR0 = (1 << FOC0) | (1 << WGM01);
		TCCR0 &= 0x88;
		TCCR0 |= ((Config_Ptr->oc0Bit & 0x03) << COM00);
		TCCR0 |= (Config_Ptr->prescaler & 0x07);
	}
}



void Timer0_setCallBack(void (*a_ptr)(void)) {
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}


void Timer0_deinit(void)
{
	TIMSK &= 0xFC;
	TCCR0 = 0;
	TCNT0 = 0;
}
