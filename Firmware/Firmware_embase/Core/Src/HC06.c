/*
 * HC06.c
 *
 *  Created on: Dec 4, 2024
 *      Author: Lenovo
 */

#include "HC06.h"

#include "string.h"

bool HC06_TestPresence()
{
	uint8_t rx_buffer[16] = {0};

	HAL_UART_Transmit(HC06_UART_HANDLE_PTR, (uint8_t *) "AT", 2, 1000);

	HAL_UART_Receive(HC06_UART_HANDLE_PTR, rx_buffer, 2, 1000);

	if (!strncmp((char *)rx_buffer, "OK", 2))
	{
		HAL_UART_Transmit(DEBUG_UART_HANDLE_PTR, (uint8_t *) "OK\n", 3, 1000);
		return true;
	}

	HAL_UART_Transmit(DEBUG_UART_HANDLE_PTR, (uint8_t *) "NO\n", 3, 1000);
	return false;
}

void HC06_SetBaudRate(HC06_BaudRate_t baud_rate)
{
	uint8_t tx_buffer[9] = {'A', 'T', '+', 'B', 'A', 'U', 'D', baud_rate + '0', 0};
	uint8_t rx_buffer[16] = {0};


	HAL_UART_Transmit(HC06_UART_HANDLE_PTR, tx_buffer, 9, 1000);

	HAL_UART_Receive(HC06_UART_HANDLE_PTR, rx_buffer, 2, 1000);

	if (!strncmp((char *)rx_buffer, "OK", 2))
	{
		HAL_UART_Transmit(DEBUG_UART_HANDLE_PTR, (uint8_t *) "OK\n", 3, 1000);
	}
	else
	{
		HAL_UART_Transmit(DEBUG_UART_HANDLE_PTR, (uint8_t *) "NO\n", 3, 1000);
	}
}
