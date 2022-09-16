/*
 * control.c
 *
 *  Created on: Jul 16, 2022
 *      Author: Mahmoud Yasser
 */

#include "control.h"

#include <avr/common.h>
#include "external_eeprom.h"
#include "uart.h"
#include "twi.h"
#include <util/delay.h>

#include "Buzzer.h"
#include "DCmotor.h"
#include "Timer0.h"

uint16 volatile g_compares_count;
uint16 volatile g_call_back_count;
uint8 volatile delay_finished_flag = 0;
uint8 password_stored_flag = 0;
uint8 g_mismatchCount = 0;

int main(void) {
	uint8 status = 0;

	SREG |= (1 << 7);
	Timer0_setCallBack(increment);

	Buzzer_Init();
	DcMotor_Init();

	UART_ConfigType uart_config;
	uart_config.baudRate = 9600;
	uart_config.characterSize = Eight_Bit_Size;
	uart_config.parity = Disabled_Parity;
	uart_config.stopBits = One_Stop_Bit;
	UART_init(&uart_config);

	I2C_ConfigType twi_config;
	twi_config.address = 1;
	twi_config.transferRate = I2C_TransferRateFastMode;
	TWI_init(&twi_config);

	while (1) {
		if (password_stored_flag == 0) {
			store_password();
			password_stored_flag = 1;
		}
		status = check_password();
		if (status == 1) /*correct password*/
		{
			g_mismatchCount = 0;
			open_motor();

		}

		else if (status == 0) /*incorrect password*/
		{
			g_mismatchCount++;
			if (g_mismatchCount == 3) {

				delay_seconds(60);
				while (delay_finished_flag == 0) {
					Buzzer_TurnOn();
				}
				delay_finished_flag = 0;

				Buzzer_TurnOff();
				g_mismatchCount = 0;
			}

		}

		else if (status == 11) /*change password and correct*/
		{
			store_password();
		}
	}

	return 0;
}

void store_password(void) {
	uint8 password[PASSWORD_SIZE];

	for (uint8 i = 0; i < PASSWORD_SIZE; i++) {
		password[i] = UART_recieveByte();
	}

	for (uint8 i = 0; i < PASSWORD_SIZE; i++) {
		EEPROM_writeByte(0x0311 + i, password[i]);
		_delay_ms(250);
	}
}

uint8 check_password(void) {
	uint8 recieved_status;
	uint8 stored_password[PASSWORD_SIZE];
	uint8 check;
	uint8 i;

	for (i = 0; i < PASSWORD_SIZE; i++) {
		EEPROM_readByte(0x0311 + i, &(stored_password[i]));
		_delay_ms(250);
	}

	check = UART_recieveByte();
	_delay_ms(1000);

	for (i = 0; i < PASSWORD_SIZE; i++) {
		UART_sendByte(stored_password[i]);
	}

	recieved_status = UART_recieveByte();
	return recieved_status;
}

void open_motor(void) {
	delay_seconds(15);
	while (delay_finished_flag == 0) {
		DcMotor_Rotate(Motor_CW);
	}
	delay_finished_flag = 0;

	delay_seconds(5);
	while (delay_finished_flag == 0) {
		DcMotor_Rotate(Motor_OFF);
	}
	delay_finished_flag = 0;

	delay_seconds(15);
	while (delay_finished_flag == 0) {
		DcMotor_Rotate(Motor_AntiCW);
	}
	delay_finished_flag = 0;

	DcMotor_Rotate(Motor_OFF);

}


void increment(void) {
	g_call_back_count++;
	if (g_call_back_count > g_compares_count) {
		delay_finished_flag = 1;
		g_call_back_count = 0;
		g_compares_count = 0;
		Timer0_deinit();
	}

}

void delay_seconds(uint32 secs) {
	Timer0_ConfigType config;
	config.mode = Timer0_CompareMode;
	config.initialCount = 0;
	config.compareModeCompareValue = 194;
	config.prescaler = Timer0_presecaler1024;
	Timer0_init(&config);
	g_compares_count = (uint16) (secs * 1000) / 25;
}

