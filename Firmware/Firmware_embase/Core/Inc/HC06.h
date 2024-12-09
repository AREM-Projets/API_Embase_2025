/*
 * HC06.h
 *
 *  Created on: Dec 4, 2024
 *      Author: Lenovo
 */

#ifndef INC_HC06_H_
#define INC_HC06_H_

#include "usart.h"
#include "stdint.h"
#include "stdbool.h"

typedef enum
{
	BAUD_RATE_1200 = 1,
	BAUD_RATE_2400 = 2,
	BAUD_RATE_4800 = 3,
	BAUD_RATE_9600 = 4,
	BAUD_RATE_19200 = 5,
	BAUD_RATE_38400 = 6,
	BAUD_RATE_57600 = 7,
	BAUD_RATE_115200 = 8
} HC06_BaudRate_t;

#define HC06_UART_HANDLE_PTR (&huart1)
#define DEBUG_UART_HANDLE_PTR (&huart2)

// the following functions should only be used BEFORE the command parser starts running, it uses both UARTs with no interrupts like a barbarian.
// This is OK, it is a configuration step and will only be used on startup.

bool HC06_TestPresence();

void HC06_SetBaudRate(HC06_BaudRate_t baud_rate);

#endif /* INC_HC06_H_ */
