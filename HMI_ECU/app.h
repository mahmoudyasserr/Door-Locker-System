/*
 * main.h
 *
 *  Created on: Jul 15, 2022
 *      Author: Mahmoud Yasser
 */

#ifndef APP_H_
#define APP_H_

#define PASSWORD_SIZE 5
uint8 compare_passwords(uint8 *password, uint8 *confirmPassword);
void set_password_widget(void);
void get_password(uint8 *password);
void increment(void);
void delay_seconds(uint32 secs);
uint8 validate_password(uint8 *password);
void open_widget(uint8 password[PASSWORD_SIZE]);
void change_password_widget(uint8 password[PASSWORD_SIZE]);
void open_or_change_pass(void);
#endif /* APP_H_ */
