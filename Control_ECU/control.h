/*
 * control.h
 *
 *  Created on: Jul 16, 2022
 *      Author: Mahmoud Yasser
 */

#ifndef CONTROL_H_
#define CONTROL_H_
#include "std_types.h"

#define PASSWORD_SIZE 5

void open_motor(void);
uint8 check_password(void);
void store_password(void);
void increment(void);
void delay_seconds(uint32 secs);
#endif /* CONTROL_H_ */
