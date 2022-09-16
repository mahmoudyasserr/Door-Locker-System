/*
 * Timer0.h
 *
 *  Created on: Jul 15, 2022
 *      Author: Mahmoud Yasser
 */

#ifndef TIMER0_H_
#define TIMER0_H_
#include "std_types.h"

typedef enum
{
	Timer0_NormalMode, Timer0_CompareMode
}Timer0_Mode;

typedef enum
{
	Timer0_noClockSource,
	Timer0_noPrescaling,
	Timer0_presecaler8,
	Timer0_presecaler64,
	Timer0_presecaler256,
	Timer0_presecaler1024,
	Timer0_externalClockSourceFallingEdge,
	Timer0_externalClockSourceRisingEdge
}Timer0_Prescaler;

typedef enum
{
	Timer0_OC0_Disconnect,
	Timer0_OC0_Toggle,
	Timer0_OC0_Clear,
	Timer0_OC0_Set
}Timer0_OC0_Bit;

typedef struct
{
	Timer0_Mode mode;
	uint8 initialCount;
	uint8 compareModeCompareValue;
	Timer0_Prescaler prescaler;
	Timer0_OC0_Bit oc0Bit;

}Timer0_ConfigType;

void Timer0_init(const Timer0_ConfigType *Config_Ptr);
void Timer0_setCallBack(void (*a_ptr)(void));
void Timer0_deinit(void);
#endif /* TIMER0_H_ */
