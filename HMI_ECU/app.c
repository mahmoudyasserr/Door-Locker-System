/*
 * main.c
 *
 *  Created on: Jul 15, 2022
 *      Author: Mahmoud Yasser
 */

#include "lcd.h"
#include "keypad.h"
#include <string.h>
#include "Timer0.h"
#include "uart.h"
#include "app.h"

#include <avr/common.h>
#include <util/delay.h>

uint8 g_mismatchCount = 0;
uint8 same_password_flag = 0;

uint16 volatile g_compares_count;
uint16 volatile g_call_back_count;
uint8 volatile delay_finished_flag = 0;

int main(void) {
	LCD_init();
	UART_ConfigType config;
	config.baudRate = 9600;
	config.characterSize = Eight_Bit_Size;
	config.parity = Disabled_Parity;
	config.stopBits = One_Stop_Bit;
	UART_init(&config);
	SREG |= (1 << 7);
	Timer0_setCallBack(increment);
	while (1) {

		while (same_password_flag == 0) {
			set_password_widget();
		}

		open_or_change_pass();
	}
	return 0;
}

uint8 compare_passwords(uint8 *password, uint8 *confirmPassword) {
	uint8 result = 1;

	for (uint8 i = 0; i < PASSWORD_SIZE; i++) {
		if (password[i] != confirmPassword[i]) {
			result = 0;
			break;
		}
	}
	return result;
}

void get_password(uint8 *password) {
	LCD_moveCursor(1, 0);
	uint8 i = 0;
	uint8 key;
	for (i = 0; i < PASSWORD_SIZE; i++) {
		key = KEYPAD_getPressedKey();

		password[i] = key;
		LCD_displayCharacter('*');

		delay_seconds(1);
		while (delay_finished_flag == 0)
			;
		delay_finished_flag = 0;

	}
}

void set_password_widget(void) {
	uint8 password[PASSWORD_SIZE];
	uint8 confirmPassword[PASSWORD_SIZE];
	LCD_clearScreen();
	LCD_displayString("Enter Password");
	get_password(password);
	LCD_clearScreen();
	LCD_displayString("Confirm Password");
	get_password(confirmPassword);

	if (compare_passwords(password, confirmPassword) == 1) {
		same_password_flag = 1;
		_delay_ms(1400);
		for (uint8 i = 0; i < PASSWORD_SIZE; i++) {
			UART_sendByte(password[i]);
		}
		LCD_clearScreen();
		LCD_displayString("Pass Set");
		_delay_ms(1400);
	}

	else {
		same_password_flag = 0;
		LCD_clearScreen();
		LCD_displayString("Mismatch");
		_delay_ms(1400);

	}

}

void open_or_change_pass(void) {
	uint8 option;
	uint8 password[PASSWORD_SIZE];

	LCD_clearScreen();
	LCD_displayString("+: Open door");
	LCD_moveCursor(1, 0);
	LCD_displayString("-: Change Pass");
	option = KEYPAD_getPressedKey();
	delay_seconds(1);
	while (delay_finished_flag == 0)
		;
	delay_finished_flag = 0;

	LCD_clearScreen();

	if (option == '+')
	{
		open_widget(password);

	}

	else if (option == '-') {
		change_password_widget(password);

	}

}

void open_widget(uint8 password[PASSWORD_SIZE]) {
	LCD_displayString("Enter Password");
	get_password(password);

	if (validate_password(password) == 1) {
		UART_sendByte(1);
		g_mismatchCount = 0;
		LCD_clearScreen();

		LCD_displayString("Opening");
		delay_seconds(15);
		while (delay_finished_flag == 0)
			;
		delay_finished_flag = 0;

		LCD_clearScreen();

		LCD_displayString("Door is Open");
		delay_seconds(5);
		while (delay_finished_flag == 0)
			;
		delay_finished_flag = 0;

		LCD_clearScreen();

		LCD_displayString("Closing");
		delay_seconds(15);
		while (delay_finished_flag == 0)
			;
		delay_finished_flag = 0;

	} else {
		UART_sendByte(0);
		g_mismatchCount++;

		if (g_mismatchCount == 3) {
			LCD_clearScreen();

			LCD_displayString("Crime Alert");
			delay_seconds(60);
			while (delay_finished_flag == 0)
				;
			delay_finished_flag = 0;
			g_mismatchCount = 0;
		}

		else {
			LCD_clearScreen();

			LCD_displayString("Wrong Password");

			delay_seconds(1);
			while (delay_finished_flag == 0)
				;
			delay_finished_flag = 0;

		}
	}
}

void change_password_widget(uint8 password[PASSWORD_SIZE]) {

	LCD_displayString("Enter password");
	get_password(password);
	if (validate_password(password) == 1) {
		UART_sendByte(11); /*password is correct*/
		same_password_flag = 0;
		while (same_password_flag == 0) {
			set_password_widget();
		}
	} else {
		UART_sendByte(0); /*password is incorrect*/
		g_mismatchCount++;
		if (g_mismatchCount == 3) {
			LCD_clearScreen();

			LCD_displayString("Crime Alert");
			delay_seconds(60);
			while (delay_finished_flag == 0)
				;
			delay_finished_flag = 0;

			g_mismatchCount = 0;
		} else {
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			delay_seconds(1);
			while (delay_finished_flag == 0)
				;
			delay_finished_flag = 0;
		}

	}

}

uint8 validate_password(uint8 *password) {

	uint8 stored_password[PASSWORD_SIZE];
	UART_sendByte(10); /*send password*/
	uint8 status = 0;
	for (uint8 i = 0; i < PASSWORD_SIZE; i++) {
		stored_password[i] = UART_recieveByte();
	}

	status = compare_passwords(password, stored_password);
	return status;
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

