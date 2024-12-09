/*
 * Uart_io.h
 *
 *  Created on: Dec 4, 2024
 *      Author: Lenovo
 */

#ifndef INC_UART_IO_H_
#define INC_UART_IO_H_

#include "usart.h"
#include "stdint.h"

void printToUart(UART_HandleTypeDef *huart, uint8_t* string);

#endif /* INC_UART_IO_H_ */
