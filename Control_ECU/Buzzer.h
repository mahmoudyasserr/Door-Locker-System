
#ifndef BUZZER_H_
#define BUZZER_H_

#include "GPIO.h"

#define BUZZER_PORT_ID 		PORTA_ID
#define BUZZER_PIN_ID  		PIN0_ID



void Buzzer_Init(void);
void Buzzer_TurnOn(void);
void Buzzer_TurnOff(void);
#endif /* BUZZER_H_ */
