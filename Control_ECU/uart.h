 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mahmoud Yasser
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	Disabled_Parity, Even_Parity = 2, Odd_Parity = 3
}UART_Parity;

typedef enum
{
	One_Stop_Bit, Two_Stop_Bits
}UART_StopBits;

typedef enum
{
	Five_Bit_Size, Six_Bit_Size, Seven_Bit_Size, Eight_Bit_Size, Nine_Bit_Size = 7
}UART_CharacterSize;



typedef struct
{
	UART_Parity parity;
	UART_StopBits stopBits;
	UART_CharacterSize characterSize;
	uint32 baudRate;
}UART_ConfigType;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
//void UART_init(uint32 baud_rate);

void UART_init(const UART_ConfigType * Config_Ptr);


/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint16 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint16 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
