/*
 * Uart_io.c
 *
 *  Created on: Dec 4, 2024
 *      Author: Lenovo
 */

#include "Uart_io.h"

#include "string.h"

/**
 * @brief print a string using UART2 via DMA. ONLY 1 PRINT PER INTERRUPT WILL WORK.
 *
 * @param string zero-terminated string to be printed
 */
void printToUart(UART_HandleTypeDef *huart, uint8_t* string)
{
	HAL_UART_Transmit_DMA(huart, string, strlen((char *) string));
}
